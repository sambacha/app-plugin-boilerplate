#include "uniswap_plugin.h"

// Prepend `dest` with `ticker`.
// Dest must be big enough to hold `ticker` + `dest` + `\0`.
static void prepend_ticker(char *dest, uint8_t destsize, char *ticker) {
    if (dest == NULL || ticker == NULL) {
        THROW(0x6503);
    }
    uint8_t ticker_len = strlen(ticker);
    uint8_t dest_len = strlen(dest);

    if (dest_len + ticker_len >= destsize) {
        THROW(0x6503);
    }

    // Right shift the string by `ticker_len` bytes.
    while (dest_len != 0) {
        dest[dest_len + ticker_len] = dest[dest_len];  // First iteration will copy the \0
        dest_len--;
    }
    // Don't forget to null terminate the string.
    dest[ticker_len] = dest[0];

    // Copy the ticker to the beginning of the string.
    memcpy(dest, ticker, ticker_len);
}

static void set_tx_type_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            PRINTF("tokenA: %s\n", context->ticker_token_a);
            strncpy(msg->title, "Liquidity pool:", msg->titleLength);
            snprintf(msg->msg, msg->msgLength, "%s / %s", context->ticker_token_a, "ETH");
            break;
        case ADD_LIQUIDITY:
        case REMOVE_LIQUIDITY:
        case REMOVE_LIQUIDITY_PERMIT:
        case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
        case REMOVE_LIQUIDITY_ETH_FEE:
        case REMOVE_LIQUIDITY_ETH_PERMIT_FEE:
            PRINTF("tokenB: %s\n", context->ticker_token_b);
            strncpy(msg->title, "Liquidity pool:", msg->titleLength);
            snprintf(msg->msg,
                     msg->msgLength,
                     "%s / %s",
                     context->ticker_token_a,
                     context->ticker_token_b);
            break;
        case SWAP_TOKENS_FOR_EXACT_TOKENS:
            PRINTF("tokenA: %s\n", context->ticker_token_a);
            strncpy(msg->title, "Swap:", msg->titleLength);
            snprintf(msg->msg,
                     msg->msgLength,
                     "%s for %s",
                     context->ticker_token_a,
                     context->ticker_token_b);
            break;
    }
}

// Set UI for "Warning" screen.
static void set_token_a_warning_ui(ethQueryContractUI_t *msg,
                                   uniswap_parameters_t *context __attribute__((unused))) {
    strncpy(msg->title, "0000 0010", msg->titleLength);
    strncpy(msg->msg, "! token A", msg->msgLength);
}

static void set_token_b_warning_ui(ethQueryContractUI_t *msg,
                                   uniswap_parameters_t *context __attribute__((unused))) {
    strncpy(msg->title, "0000 1000", msg->titleLength);
    strncpy(msg->msg, "! token B", msg->msgLength);
}

static void set_amount_token_a_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
        case ADD_LIQUIDITY:
            strncpy(msg->title, "Deposit:", msg->titleLength);
            break;
        case REMOVE_LIQUIDITY:
        case REMOVE_LIQUIDITY_PERMIT:
        case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
        case REMOVE_LIQUIDITY_ETH_FEE:
        case REMOVE_LIQUIDITY_ETH_PERMIT_FEE:
            strncpy(msg->title, "Remove:", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    adjustDecimals(
        (char *) context->token_a_amount_sent,
        strnlen((char *) context->token_a_amount_sent, sizeof(context->token_a_amount_sent)),
        msg->msg,
        msg->msgLength,
        context->decimals_token_a);
    prepend_ticker(msg->msg, msg->msgLength, context->ticker_token_a);
}

static void set_amount_token_b_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
        case ADD_LIQUIDITY:
            strncpy(msg->title, "Deposit:", msg->titleLength);
            break;
        case REMOVE_LIQUIDITY:
        case REMOVE_LIQUIDITY_PERMIT:
        case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
        case REMOVE_LIQUIDITY_ETH_FEE:
        case REMOVE_LIQUIDITY_ETH_PERMIT_FEE:
            strncpy(msg->title, "Remove:", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    adjustDecimals(
        (char *) context->token_b_amount_sent,
        strnlen((char *) context->token_b_amount_sent, sizeof(context->token_b_amount_sent)),
        msg->msg,
        msg->msgLength,
        context->decimals_token_b);
    prepend_ticker(msg->msg, msg->msgLength, context->ticker_token_b);
}

static void set_amount_eth_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            strncpy(msg->title, "Deposit:", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    amountToString((uint8_t *) msg->pluginSharedRO->txContent->value.value,
                   msg->pluginSharedRO->txContent->value.length,
                   WEI_TO_ETHER,
                   "ETH ",
                   msg->msg,
                   msg->msgLength);
}

static void set_beneficiary_warning_ui(ethQueryContractUI_t *msg,
                                       uniswap_parameters_t *context __attribute__((unused))) {
    strncpy(msg->title, "0010 0000", msg->titleLength);
    strncpy(msg->msg, "Not user's address", msg->titleLength);
}

// Set UI for "Beneficiary" screen.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    strncpy(msg->title, "Beneficiary:", msg->titleLength);
    msg->msg[0] = '0';
    msg->msg[1] = 'x';
    chain_config_t chainConfig = {0};
    getEthAddressStringFromBinary((uint8_t *) context->beneficiary,
                                  (uint8_t *) msg->msg + 2,
                                  msg->pluginSharedRW->sha3,
                                  &chainConfig);
}

// Not used if last bit in screen array isn't 1
static void set_last_ui(ethQueryContractUI_t *msg,
                        uniswap_parameters_t *context __attribute__((unused))) {
    strncpy(msg->title, "1000 0000", msg->titleLength);
    strncpy(msg->msg, "LAST", msg->titleLength);
}

static void skip_right(ethQueryContractUI_t *msg __attribute__((unused)),
                       uniswap_parameters_t *context) {
    while (!(context->screen_array & context->plugin_screen_index << 1)) {
        context->plugin_screen_index <<= 1;
    }
}

static void skip_left(ethQueryContractUI_t *msg __attribute__((unused)),
                      uniswap_parameters_t *context) {
    while (!(context->screen_array & context->plugin_screen_index >> 1)) {
        context->plugin_screen_index >>= 1;
    }
}

static bool get_scroll_direction(uint8_t screen_index, uint8_t previous_screen_index) {
    if (screen_index > previous_screen_index || screen_index == 0)
        return RIGHT_SCROLL;
    else
        return LEFT_SCROLL;
}

static void get_screen_array(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    if (msg->screenIndex == 0) {
        context->plugin_screen_index = TX_TYPE_UI;
        context->previous_screen_index = 0;
        return;
    }
    // This should only happen on last valid Screen
    if (msg->screenIndex == context->previous_screen_index) {
        context->plugin_screen_index = LAST_UI;
        if (context->screen_array & LAST_UI) return;
    }
    bool scroll_direction = get_scroll_direction(msg->screenIndex, context->previous_screen_index);
    // Save previous_screen_index after all checks are done.
    context->previous_screen_index = msg->screenIndex;
    // Scroll to next screen
    if (scroll_direction == RIGHT_SCROLL) {
        skip_right(msg, context);
        context->plugin_screen_index <<= 1;
    } else {
        skip_left(msg, context);
        context->plugin_screen_index >>= 1;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    get_screen_array(msg, context);
    print_bytes(&context->plugin_screen_index, 1);
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;
    switch (context->plugin_screen_index) {
        case TX_TYPE_UI:
            PRINTF("GPIRIOU TX_TYPE\n");
            set_tx_type_ui(msg, context);
            break;
        case WARNING_TOKEN_A_UI:
            PRINTF("GPIRIOU WARNING A\n");
            set_token_a_warning_ui(msg, context);
            break;
        case AMOUNT_TOKEN_A_UI:
            PRINTF("GPIRIOU AMOUNT A\n");
            set_amount_token_a_ui(msg, context);
            break;
        case WARNING_TOKEN_B_UI:
            PRINTF("GPIRIOU WARNING B\n");
            set_token_b_warning_ui(msg, context);
            break;
        case AMOUNT_TOKEN_B_UI:
            if (context->selectorIndex == ADD_LIQUIDITY_ETH) {
                PRINTF("GPIRIOU AMOUNT ETH\n");
                set_amount_eth_ui(msg, context);
            } else {
                PRINTF("GPIRIOU AMOUNT B\n");
                set_amount_token_b_ui(msg, context);
            }
            break;
        case WARNING_ADDRESS_UI:
            PRINTF("GPIRIOU WARNING ADDRESS\n");
            set_beneficiary_warning_ui(msg, context);
            break;
        case ADDRESS_UI:
            PRINTF("GPIRIOU BENEFICIARY\n");
            set_beneficiary_ui(msg, context);
            break;
        case LAST_UI:
            PRINTF("GPIRIOU LAST UI\n");
            set_last_ui(msg, context);
            break;
        default:
            PRINTF("GPIRIOU ERROR\n");
            break;
    }
}