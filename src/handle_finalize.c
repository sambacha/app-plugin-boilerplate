#include "sushiswap_plugin.h"

static void check_beneficiary_warning(ethPluginFinalize_t *msg, sushiswap_parameters_t *context) {
    if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH)) {
        PRINTF("DEVELOPER WARNING SET\n");
        context->screen_array |= WARNING_ADDRESS_UI;
        msg->numScreens++;
    }
}

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    sushiswap_parameters_t *context = (sushiswap_parameters_t *) msg->pluginContext;

    // set generic screen_array
    context->screen_array |= TX_TYPE_UI;
    context->screen_array |= AMOUNT_TOKEN_A_UI;
    context->screen_array |= AMOUNT_TOKEN_B_UI;
    context->screen_array |= ADDRESS_UI;

    if (context->valid) {
        msg->uiType = ETH_UI_TYPE_GENERIC;
        context->plugin_screen_index = TX_TYPE_UI;
        msg->numScreens = 4;

        check_beneficiary_warning(msg, context);

        msg->tokenLookup1 = context->token_a_address;  // TODO: CHECK THIS
        msg->tokenLookup2 = context->token_b_address;  // TODO: CHECK THIS
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Invalid context\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}