#include "uniswap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;
    PRINTF("handle finalize\n");
    if (context->valid) {
        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->numScreens = 2;

        msg->tokenLookup1 = context->contract_token_address;  // TODO: CHECK THIS
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Invalid context\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}
