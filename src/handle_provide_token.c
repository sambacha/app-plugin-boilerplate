#include "uniswap_plugin.h"

void handle_provide_token(void *parameters) {
    ethPluginProvideToken_t *msg = (ethPluginProvideToken_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;
    PRINTF("plugin provide token: 0x%p, 0x%p\n", msg->token1, msg->token2);

    if (msg->token1) {
        context->decimals_token = msg->token1->decimals;
        strncpy(context->ticker_token, (char *) msg->token1->ticker, sizeof(context->ticker_token));

        // Keep track that we found the token.
        context->tokens_found |= TOKEN_FOUND;
    } else {
        context->decimals_token = DEFAULT_DECIMAL;
        strncpy(context->ticker_token, DEFAULT_TICKER, sizeof(context->ticker_token));

        // We will need an additional screen to display a warning message.
        msg->additionalScreens++;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}