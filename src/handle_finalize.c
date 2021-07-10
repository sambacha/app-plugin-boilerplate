#include "uniswap_plugin.h"

static void screen_array(uniswap_parameters_t *context) {
}

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    // print_bytes(msg->address, ADDRESS_LENGTH);

    // PENZO full random placement
    context->screen_array |= TX_TYPE_UI;

    if (context->valid) {
        msg->uiType = ETH_UI_TYPE_GENERIC;
        switch (context->selectorIndex) {
            case ADD_LIQUIDITY_ETH:
            case ADD_LIQUIDITY:
            case REMOVE_LIQUIDITY_ETH:
                context->plugin_screen_index = TX_TYPE_UI;
                msg->numScreens = 4;
                break;
                break;
                break;
        }

        if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH)) {
            context->screen_array |= WARNING_ADDRESS_UI;
            msg->numScreens++;
        }

        msg->tokenLookup1 = context->token_a_address;  // TODO: CHECK THIS
        msg->tokenLookup2 = context->token_b_address;  // TODO: CHECK THIS
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Invalid context\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}