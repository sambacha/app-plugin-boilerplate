#include "uniswap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;
    PRINTF("handle finalize\n");

    uint8_t user_address[ADDRESS_LENGTH];
    context->should_warn = 0;

    get_public_key(msg->pluginSharedRO->pathLength, msg->pluginSharedRO->bip32Path, 
                    msg->pluginSharedRW->sha3, user_address, sizeof(user_address));

    if (memcmp(user_address, context->beneficiary, sizeof(user_address)))
        context->should_warn = 1;
    
    if (context->valid) {
        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->numScreens = 3;
        if (context->should_warn)
            msg->numScreens++;

        msg->tokenLookup1 = context->contract_token_address;  // TODO: CHECK THIS
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Invalid context\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}
