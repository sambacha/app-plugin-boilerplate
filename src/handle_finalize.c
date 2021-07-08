#include "uniswap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    uint8_t user_address[ADDRESS_LENGTH];
    context->should_warn = 0;

    print_bytes(msg->address, ADDRESS_LENGTH);

    if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH))
        context->should_warn = 1;

    if (context->valid) {
        msg->uiType = ETH_UI_TYPE_GENERIC;
        switch (context->selectorIndex) {
            case ADD_LIQUIDITY_ETH:
            case ADD_LIQUIDITY:
                msg->numScreens = 4;
                break;
                break;
        }
        if (context->should_warn) 
            msg->numScreens++;

        msg->tokenLookup1 = context->token_a_address;  // TODO: CHECK THIS
        msg->tokenLookup2 = context->token_b_address;  // TODO: CHECK THIS
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Invalid context\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}
