/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"
#include "uniswap_plugin.h"

// Define here all the selectors you wish to support.

static const uint8_t UNISWAP_ADD_LIQUIDITY[SELECTOR_SIZE] = {0xe8, 0xe3, 0x37, 0x00};
static const uint8_t UNISWAP_ADD_LIQUIDITY_ETH[SELECTOR_SIZE] = {0xf3, 0x05, 0xd7, 0x19};
static const uint8_t UNISWAP_REMOVE_LIQUIDITY_ETH[SELECTOR_SIZE] = {0x02, 0x75, 0x1c, 0xec};
static const uint8_t UNISWAP_REMOVE_LIQUIDITY_ETH_PERMIT[SELECTOR_SIZE] = {0xde, 0xd9, 0x38, 0x2a};
// static const uint8_t UNISWAP_REMOVE_LIQUIDITY[SELECTOR_SIZE] = {0xe2, 0x75, 0x1c, 0xec};
static const uint8_t UNISWAP_REMOVE_LIQUIDITY_ETH_FEE[SELECTOR_SIZE] = {0xaf, 0x29, 0x79, 0xeb};
static const uint8_t UNISWAP_REMOVE_LIQUIDITY_ETH_PERMIT_FEE[SELECTOR_SIZE] = {0x5b,
                                                                               0x0d,
                                                                               0x59,
                                                                               0x84};

// Array of all the different uniswap selectors. Make sure this follows the same order as the
// enum defined in `uniswap_plugin.h`
const uint8_t *const UNISWAP_SELECTORS[NUM_UNISWAP_SELECTORS] = {
    UNISWAP_ADD_LIQUIDITY,
    UNISWAP_ADD_LIQUIDITY_ETH,
    UNISWAP_REMOVE_LIQUIDITY_ETH,
    UNISWAP_REMOVE_LIQUIDITY_ETH_PERMIT,
    UNISWAP_REMOVE_LIQUIDITY_ETH_FEE,
    UNISWAP_REMOVE_LIQUIDITY_ETH_PERMIT_FEE,
    //    UNISWAP_REMOVE_LIQUIDITY,
};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    PRINTF("just in: message: %d\n", message);
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            PRINTF("PROUT INIT\n");
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            PRINTF("PROVIDE\n");
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            PRINTF("GPIRIOU FINALIZE\n");
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_TOKEN:
            PRINTF("GPIRIOU PROVIDE\n");
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            PRINTF("GPIROU ID\n");
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            PRINTF("UI\n");
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
    }
}

// Calls the ethereum app.
void call_app_ethereum() {
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int) "Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *) &libcall_params);
}

// Weird low-level black magic.
__attribute__((section(".boot"))) int main(int arg0) {
    // Low-level black magic, don't touch.
    // exit critical section
    __asm volatile("cpsie i");

    // Low-level black magic, don't touch.
    // ensure exception will work as planned
    os_boot();

    // Try catch block. Please read the docs for more information on how to use those!
    BEGIN_TRY {
        TRY {
            // Low-level black magic. Don't touch.
            check_api_level(CX_COMPAT_APILEVEL);

            // Check if we are called from the dashboard.
            if (!arg0) {
                // called from dashboard, launch Ethereum app
                call_app_ethereum();
                return 0;
            } else {
                // not called from dashboard: called from the ethereum app!
                unsigned int *args = (unsigned int *) arg0;

                // If `ETH_PLUGIN_CHECK_PRESENCE`, we can skip `dispatch_plugin_calls`.
                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE) {
                    dispatch_plugin_calls(args[0], (void *) args[1]);
                }

                // Call `os_lib_end`, go back to the ethereum app.
                os_lib_end();
            }
        }
        FINALLY {
        }
    }
    END_TRY;

    // Will not get reached.
    return 0;
}
