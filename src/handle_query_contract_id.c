#include "uniswap_plugin.h"

void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    strncpy(msg->name, PLUGIN_NAME, msg->nameLength);

    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
        case ADD_LIQUIDITY:
            strncpy(msg->version, "Add liquidity", msg->versionLength);
            break;
        // case REMOVE_LIQUIDITY_ETH_WITH_PERMIT_SUPPORTING_FEE_ON_TRANSFER_TOKENS:
        // case REMOVE_LIQUIDITY_ETH_SUPPORTING_FEE_ON_TRANSFER_TOKENS:
        // case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
            strncpy(msg->version, "Remove liquidity", msg->versionLength);
            break;
        default:
            PRINTF("Selector Index :%d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}