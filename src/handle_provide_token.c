#include "uniswap_plugin.h"

void handle_provide_token(void *parameters) {
    ethPluginProvideToken_t *msg = (ethPluginProvideToken_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;
    PRINTF("plugin provide token: 0x%p, 0x%p\n", msg->token1, msg->token2);

    context->screen_array |= AMOUNT_TOKEN_A_UI;
    context->screen_array |= AMOUNT_TOKEN_B_UI;
    context->screen_array |= ADDRESS_UI;
    if (msg->token1) {
        context->decimals_token = msg->token1->decimals;
        strncpy(context->ticker_token_a,
                (char *) msg->token1->ticker,
                sizeof(context->ticker_token_a));
    } else {
        context->decimals_token = DEFAULT_DECIMAL;
        strncpy(context->ticker_token_a, DEFAULT_TICKER, sizeof(context->ticker_token_a));

        // We will need an additional screen to display a warning message.
        context->screen_array |= WARNING_TOKEN_A_UI;
        msg->additionalScreens++;
    }

//    PRINTF("GPIRIOU token2:%s\n", msg->token2);
    if (msg->token2 || context->selectorIndex == ADD_LIQUIDITY_ETH) {
        PRINTF("GPIRIOU TEEEEEEEEEEST\n");
        if (msg->token2) {
            context->decimals_token = msg->token2->decimals;
            strncpy(context->ticker_token_b,
                    (char *) msg->token2->ticker,
                    sizeof(context->ticker_token_b));
        }
    } else {
        switch (context->selectorIndex) {
            case REMOVE_LIQUIDITY_ETH_PERMIT:
                context->decimals_token = 18;
                strncpy(context->ticker_token_b, "WETH ", 5);
                break;
            default:
                PRINTF("GPIRIOU PROVIDE ELSE\n");
                context->decimals_token = DEFAULT_DECIMAL;
                strncpy(context->ticker_token_b, DEFAULT_TICKER, sizeof(context->ticker_token_b));
                context->screen_array |= WARNING_TOKEN_B_UI;
                msg->additionalScreens++;
        }

    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}