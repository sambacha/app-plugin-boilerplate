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

// Set UI for the "Send" screen.
static void set_send_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            strncpy(msg->title, "Send", msg->titleLength);
            break;
        case ADD_LIQUIDITY:
            strncpy(msg->title, "Send Max", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    adjustDecimals(
        (char *) context->token_amount_deposited,
        strnlen((char *) context->token_amount_deposited, sizeof(context->token_amount_deposited)),
        msg->msg,
        msg->msgLength,
        context->decimals_token);

    prepend_ticker(msg->msg, msg->msgLength, context->ticker_token);
}

// Set UI for "Receive" screen.
static void set_amount_token_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            strncpy(msg->title, "Deposit", msg->titleLength);
            break;
        // case UNISWAP_DUMMY_2:
        //    strncpy(msg->title, "Receive", msg->titleLength);
        //    break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    adjustDecimals(
        (char *) context->token_amount_deposited,
        strnlen((char *) context->token_amount_deposited, sizeof(context->token_amount_deposited)),
        msg->msg,
        msg->msgLength,
        context->decimals_token);

    prepend_ticker(msg->msg, msg->msgLength, context->ticker_token);
}

// Set UI for "Receive" screen.
static void set_amount_eth_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            strncpy(msg->title, "Deposit", msg->titleLength);
            break;
        // case UNISWAP_DUMMY_2:
        //    strncpy(msg->title, "Receive", msg->titleLength);
        //    break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    amountToString((uint8_t *) msg->pluginSharedRO->txContent->value.value,
                   msg->pluginSharedRO->txContent->value.length,  // value.length
                   WEI_TO_ETHER,
                   "ETH ",
                   msg->msg,
                   msg->msgLength);
}

// Set UI for "Beneficiary" screen.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    strncpy(msg->title, "Beneficiary", msg->titleLength);

    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    chain_config_t chainConfig = {0};

    getEthAddressStringFromBinary((uint8_t *) context->beneficiary,
                                  (uint8_t *) msg->msg + 2,
                                  msg->pluginSharedRW->sha3,
                                  &chainConfig);
}

// Set UI for "Warning" screen.
static void set_warning_ui(ethQueryContractUI_t *msg,
                           uniswap_parameters_t *context __attribute__((unused))) {
    strncpy(msg->title, "WARNING", msg->titleLength);
    strncpy(msg->msg, "Unknown token", msg->msgLength);
}

static void set_pool_ui(ethQueryContractUI_t *msg, uniswap_parameters_t *context) {
    strncpy(msg->title, "Pool:", msg->titleLength);
    snprintf(msg->msg, msg->msgLength, "%s / %s", context->ticker_token, "ETH");
}

static void check_if_address_is_user_address(ethQueryContractUI_t *msg,
                                             uniswap_parameters_t *context) {
    char user_address[ADDRESS_LENGTH];

    // get_public_key(msg->pluginSharedRO->bip32Path,
    // sizeof(msg->pluginSharedRO->bip32Path),
    // msg->pluginSharedRW->sha3,
    // user_address, ADDRESS_LENGTH);

    if (memcmp(context->beneficiary, user_address, ADDRESS_LENGTH)) {
        strncpy(msg->title, "WARNING", msg->titleLength);
        strncpy(msg->msg, "Not user's address", msg->titleLength);
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (msg->screenIndex) {
        case 0:
            if (context->tokens_found & TOKEN_FOUND)
                set_pool_ui(msg, context);
            else
                set_warning_ui(msg, context);
            break;
        case 1:
            set_amount_token_ui(msg, context);
            break;
        case 2:
            set_amount_eth_ui(msg, context);
            break;
        case 3:

            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
