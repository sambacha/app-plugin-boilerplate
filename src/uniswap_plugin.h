#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Size of parameters passed in by the Ethereum app. No need to modify it. TODO: move it to
// `eth_plugin_interals.h`.
#define PARAMETER_LENGTH 32

// Size of the smart-contract method. No need to modify it. TODO: mov eit to
// `eth_plugin_interals.h`.
#define SELECTOR_SIZE 4

// Value to be passed in as parameter when calling the Ethereum app. TODO: mov eit to
// `eth_plugin_interals.h`.
#define RUN_APPLICATION 1

// Name of the plugin.
#define PLUGIN_NAME "Uniswap"

// Number of selectors defined in this plugin.
#define NUM_UNISWAP_SELECTORS 12

// Enumeration of the different selectors possible.
// Should follow the array declared in main.c
typedef enum {
    ADD_LIQUIDITY,
    ADD_LIQUIDITY_ETH,
    REMOVE_LIQUIDITY,
    REMOVE_LIQUIDITY_PERMIT,
    REMOVE_LIQUIDITY_ETH,
    REMOVE_LIQUIDITY_ETH_PERMIT,
    REMOVE_LIQUIDITY_ETH_FEE,
    REMOVE_LIQUIDITY_ETH_PERMIT_FEE,
    SWAP_EXACT_TOKENS_FOR_TOKENS,
    SWAP_EXACT_TOKENS_FOR_TOKENS_FEE,
    SWAP_TOKENS_FOR_EXACT_ETH,
    SWAP_TOKENS_FOR_EXACT_TOKENS,
} uniswapSelector_t;

extern const uint8_t *const UNISWAP_SELECTORS[NUM_UNISWAP_SELECTORS];

// screeen array correspondance
#define TX_TYPE_UI         1
#define WARNING_TOKEN_A_UI (1 << 1)
#define AMOUNT_TOKEN_A_UI  (1 << 2)
#define WARNING_TOKEN_B_UI (1 << 3)
#define AMOUNT_TOKEN_B_UI  (1 << 4)
#define WARNING_ADDRESS_UI (1 << 5)
#define ADDRESS_UI         (1 << 6)
#define LAST_UI            (1 << 7)

#define RIGHT_SCROLL 1
#define LEFT_SCROLL  0

// Enumeration used to parse the smart-contract data.
typedef enum {
    TOKEN_A_ADDRESS,
    TOKEN_B_ADDRESS,
    LIQUIDITY,
    AMOUNT_TOKEN_A,
    AMOUNT_TOKEN_B,
    AMOUNT_TOKEN_A_MIN,
    AMOUNT_TOKEN_B_MIN,
    AMOUNT_ETH_MIN,
    AMOUNT_OUT,
    AMOUNT_IN,
    AMOUNT_IN_MAX,
    PATH_OFFSET,
    BENEFICIARY,
    DEADLINE,
    NONE,
} selectorField;

#define WETH_TICKER   "WETH"
#define WETH_DECIMALS 18

// Number of decimals used when the token wasn't found in the Crypto Asset List.
#define DEFAULT_DECIMAL WEI_TO_ETHER

// Ticker used when the token wasn't found in the Crypto Asset List.
#define DEFAULT_TICKER "? "

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct uniswap_parameters_t {
    uint8_t token_a_address[ADDRESS_LENGTH];     // 20
    uint8_t token_b_address[ADDRESS_LENGTH];     // 20
    uint8_t token_a_amount_sent[INT256_LENGTH];  // 32
    uint8_t token_b_amount_sent[INT256_LENGTH];  // 32
    char ticker_token_a[MAX_TICKER_LEN];         // 12
    char ticker_token_b[MAX_TICKER_LEN];         // 12
    uint8_t beneficiary[ADDRESS_LENGTH];         // 20
    uint8_t screen_array;                        // 1
    uint8_t previous_screen_index;               // 1
    uint8_t plugin_screen_index;                 // 1

    uint16_t path_offset;      // 2
    uint8_t skip;              // 1
    uint8_t next_param;        // 1
    uint8_t valid;             // 1
    uint8_t decimals_token_a;  // 1
    uint8_t decimals_token_b;  // 1
    uint8_t selectorIndex;     // 1
    // = 159
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