#include "uniswap_plugin.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_1) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // TODO: this could be removed as this can be checked statically?
    if (msg->pluginContextLength < sizeof(uniswap_parameters_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));
    // Mark context as valid.
    context->valid = 1;

    // Look for the index of the selectorIndex passed in by `msg`.
    uint8_t i;
    for (i = 0; i < NUM_UNISWAP_SELECTORS; i++) {
        if (memcmp((uint8_t *) PIC(UNISWAP_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            context->selectorIndex = i;
            break;
        }
    }

    // If `i == NUM_UNISWAP_SELECTOR` it means we haven't found the selector. Return an error.
    if (i == NUM_UNISWAP_SELECTORS) {
        context->valid = 0;
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
    }

    // Set `next_param` to be the first field we expect to parse.
    PRINTF("selector: %u", context->selectorIndex);
    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
        case ADD_LIQUIDITY:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
            context->next_param = TOKEN_A_ADDRESS;
            break;
            break;
        default:
            PRINTF("Missing selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
