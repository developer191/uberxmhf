/*
 * @XMHF_LICENSE_HEADER_START@
 *
 * eXtensible, Modular Hypervisor Framework (XMHF)
 * Copyright (c) 2009-2012 Carnegie Mellon University
 * Copyright (c) 2010-2012 VDG Inc.
 * All Rights Reserved.
 *
 * Developed by: XMHF Team
 *               Carnegie Mellon University / CyLab
 *               VDG Inc.
 *               http://xmhf.org
 *
 * This file is part of the EMHF historical reference
 * codebase, and is released under the terms of the
 * GNU General Public License (GPL) version 2.
 * Please see the LICENSE file for details.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @XMHF_LICENSE_HEADER_END@
 */

/**
 * Consumable interface to CTR_DRBG PRNG.
 *
 * TODO: Support re-seeding.
 */

#include <target.h>
#include <random.h>
#include <crypto_init.h>

/**
 * Reseed the CTR_DRBG if needed.  This function is structured to do
 * nothing if a reseed is not required, to simplify the logic in the
 * functions that actually service requests for random bytes.
 *
 * returns: 0 on success
 */
int reseed_ctr_drbg_using_tpm_entropy_if_needed(void) {
    uint8_t EntropyInput[CTR_DRBG_SEED_BITS/8];

    ASSERT(true == g_master_crypto_init_completed);
    
    if (g_drbg.reseed_counter < NIST_CTR_DRBG_RESEED_INTERVAL)
        return 0; /* nothing to do */

    dprintf(LOG_ERROR, "\n[TV] Low Entropy: Attemping TPM-based PRNG reseed.\n");    
    
    /* Get CTR_DRBG_SEED_BITS of entropy from the hardware TPM */
    if(get_hw_tpm_entropy(EntropyInput, CTR_DRBG_SEED_BITS/8)) {
        dprintf(LOG_ERROR, "\nFATAL ERROR: Could not access TPM to reseed PRNG.\n");
        HALT();
    }

    nist_ctr_drbg_reseed(&g_drbg, EntropyInput, sizeof(EntropyInput), NULL, 0);
    
    dprintf(LOG_TRACE, "\n[TV] master_crypto_init: PRNG reseeded successfully.\n");

    return 0;
}


/**
 * Returns a pseudo-random byte or HALT's the whole system if one
 * cannot be generated.
 */
uint8_t rand_byte_or_die(void) {
    uint8_t byte;
    int rv;
    
    if(!g_master_crypto_init_completed) {
        dprintf(LOG_ERROR, "\nFATAL: !g_master_crypto_init_completed\n");
        HALT();
    }

    reseed_ctr_drbg_using_tpm_entropy_if_needed();
    if((rv = nist_ctr_drbg_generate(&g_drbg, &byte, sizeof(byte), NULL, 0))) {
        dprintf(LOG_ERROR, "\nFATAL: %s: nist_ctr_drbg_generate() returned error %d!\n", __FUNCTION__, rv);
        HALT();
    }

    return byte;
}

/**
 * Populates a buffer of pseudo-random bytes or HALT's the whole
 * system if they cannot be generated.
 */
void rand_bytes_or_die(uint8_t *out, unsigned int len) {
    int rv;
    if(!g_master_crypto_init_completed) {
        dprintf(LOG_ERROR, "FATAL: !g_master_crypto_init_completed\n");
        HALT();
    }

    if(!out || (len < 1)) {
        dprintf(LOG_ERROR, "FATAL: !out || (len < 1)\n");
        HALT();
    }
    
    reseed_ctr_drbg_using_tpm_entropy_if_needed();
    if((rv = nist_ctr_drbg_generate(&g_drbg, out, len, NULL, 0))) {
        dprintf(LOG_ERROR, "\nFATAL: %s: nist_ctr_drbg_generate() returned error %d!\n", __FUNCTION__, rv);
        HALT();
    }
}    

/**
 * Best effort to populate an array of *len random-bytes.  Returns 0
 * on success, otherwise returns the number of bytes that were
 * actually available (and updates *len).
 */
int rand_bytes(uint8_t *out, unsigned int *len) {
    int rv;
    /* even here we do not want to tolerate failure to initialize */
    if(!g_master_crypto_init_completed) {
        dprintf(LOG_ERROR, "FATAL: !g_master_crypto_init_completed\n");
        HALT();
    }

    if(!out || !len || *len < 1) {
        dprintf(LOG_ERROR, "ERROR: !out || !len || *len < 1\n");
        return 1;
    }

    /* at the present time this will either give all requested bytes
     * or fail completely.  no support for partial returns, though
     * that may one day be desirable. */
    reseed_ctr_drbg_using_tpm_entropy_if_needed();
    if((rv = nist_ctr_drbg_generate(&g_drbg, out, *len, NULL, 0))) {
        dprintf(LOG_ERROR, "\nFATAL: %s: nist_ctr_drbg_generate() returned error %d!\n", __FUNCTION__, rv);
        return 1;
    }

    dprintf(LOG_TRACE, "\n[TV]Successfully generated %d pseudo-random bytes\n", *len);

    return 0;                              
}


