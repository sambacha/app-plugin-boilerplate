#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Size of parameters passed in by the Ethereum app. No need to modify it. TODO: mov eit to
// `eth_plugin_interals.h`.
#define PARAMETER_LENGTH 32

// Size of the smart-contract method. No need to modify it. TODO: mov eit to
// `eth_plugin_interals.h`.
#define SELECTOR_SIZE 4

// Value to be passed in as parameter when calling the Ethereum app. TODO: mov eit to
// `eth_plugin_interals.h`.
#define RUN_APPLICATION 1

// Number of selectors defined in this plugin.
#define NUM_UNISWAP_SELECTORS 2

// Name of the plugin.
#define PLUGIN_NAME "Uniswap"

// TODO: add doc.
#define TOKEN_FOUND        1
#define AMOUNT_TOKEN_FOUND (1 << 1)

// Enumeration of the different selectors possible.
// Should follow the array declared in main.c
typedef enum {
    ADD_LIQUIDITY_ETH,
    ADD_LIQUIDITY,
} uniswapSelector_t;

// Enumeration used to parse the smart-contract data.
typedef enum {
    TOKEN,
    AMOUNT_TOKEN,
    AMOUNT_TOKEN_MIN,
    AMOUNT_ETH_MIN,
    BENEFICIARY,
    DEADLINE,
    NONE,
} selectorField;

// Enumeration of different screens that the plugin might display.
typedef enum {
    SEND_SCREEN,
    RECEIVE_SCREEN,
    BENEFICIARY_SCREEN,
    WARN_SCREEN,
    ERROR,  // This variant indicates that an error occured. No display should occur.
} screens_t;

extern const uint8_t *const UNISWAP_SELECTORS[NUM_UNISWAP_SELECTORS];

// Number of decimals used when the token wasn't found in the Crypto Asset List.
#define DEFAULT_DECIMAL WEI_TO_ETHER

// Ticker used when the token wasn't found in the Crypto Asset List.
#define DEFAULT_TICKER ""

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct uniswap_parameters_t {
    //// uint8_t token_amount_deposited[INT256_LENGTH];
    uint8_t token_amount_deposited[INT256_LENGTH];
    char beneficiary[ADDRESS_LENGTH];
    uint8_t contract_token_address[ADDRESS_LENGTH];
    uint8_t contract_address_received[ADDRESS_LENGTH];
    char ticker_token[MAX_TICKER_LEN];
    char ticker_received[MAX_TICKER_LEN];
    bool should_warn;

    uint8_t next_param;
    uint8_t tokens_found;
    uint8_t valid;
    uint8_t decimals_token;
    uint8_t decimals_received;
    uint8_t selectorIndex;
} uniswap_parameters_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
// TODO: unify this with the check in ethPluginInitContract.
_Static_assert(sizeof(uniswap_parameters_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);