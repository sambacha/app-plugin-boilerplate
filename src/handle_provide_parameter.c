#include "uniswap_plugin.h"

// Store the amount sent in the form of a string, without any ticker or
// decimals. These will be added when displaying.
static void handle_amount_eth(ethPluginProvideParameter_t *msg,
                               uniswap_parameters_t *context) {
    memset(context->token_amount_deposited, 0, sizeof(context->token_amount_deposited));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,
                   "",
                   (char *) context->token_amount_deposited,
                   sizeof(context->token_amount_deposited));
    PRINTF("AMOUNT SENT: %s\n", context->token_amount_deposited);
}

// Store the token amount deposited in the form of a string, without any ticker or
// decimals. These will be added when displaying.
static void handle_token_amount(ethPluginProvideParameter_t *msg,
                                   uniswap_parameters_t *context) {
    memset(context->token_amount_deposited, 0, sizeof(context->token_amount_deposited));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,   // No decimals
                   "",  // No ticker
                   (char *) context->token_amount_deposited,
                   sizeof(context->token_amount_deposited));
    PRINTF("TOKEN AMOUNT DEPOSITED: %s\n", context->token_amount_deposited);
}

static void handle_beneficiary(ethPluginProvideParameter_t *msg,
                               uniswap_parameters_t *context) {
    memset(context->beneficiary, 0, sizeof(context->beneficiary));
    memcpy(context->beneficiary,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_token(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->contract_token_address, 0, sizeof(context->contract_token_address));
    memcpy(context->contract_token_address,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->contract_token_address));
    PRINTF("TOKEN CONTRACT: %.*H\n", ADDRESS_LENGTH, context->contract_token_address);
}

//static void handle_token_amount(ethPluginProvideParameter_t *msg,
//                                  uniswap_parameters_t *context) {
//    memset(context->contract_address_received, 0, sizeof(context->contract_address_received));
//    memcpy(context->contract_address_received,
//           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
//           sizeof(context->contract_address_received));
//    PRINTF("TOKEN RECIEVED: %.*H\n", ADDRESS_LENGTH, context->contract_address_received);
//}

static void handle_add_liquidity_eth(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    // Describe ABI
    switch (context->next_param) {
        case TOKEN:  // deposited token address
            handle_token(msg, context);
            context->next_param = AMOUNT_TOKEN;
            break;
        case AMOUNT_TOKEN:  // token amount deposited
            handle_token_amount(msg, context);
            PRINTF("after\n");
            context->next_param = AMOUNT_TOKEN_MIN;
            break;
        case AMOUNT_TOKEN_MIN:  // not used
            PRINTF("token min\n");
            context->next_param = AMOUNT_ETH_MIN;
            break;
        case AMOUNT_ETH_MIN:  // not used
            PRINTF("eth min\n");
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // address receiving liquitity tokens
            PRINTF("benef min\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:  // not used
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

//static void handle_dummy_two(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
//    // Describe ABI
//    switch (context->next_param) {
//        case AMOUNT_TOKEN:  // fromToken
//            handle_token_amount(msg, context);
//            context->next_param = TOKEN;
//            break;
//        case TOKEN:  // toToken
//            handle_token(msg, context);
//            context->next_param = AMOUNT_ETH_MIN;
//            break;
//        case AMOUNT_ETH_MIN:  // fromAmount
//            handle_token_amount(msg, context);
//            context->next_param = AMOUNT_TOKEN_MIN;
//            break;
//        case AMOUNT_TOKEN_MIN:  // toAmount
//            handle_amount_eth(msg, context);
//            context->next_param = BENEFICIARY;
//            break;
//        case BENEFICIARY:  // beneficiary
//            handle_beneficiary(msg, context);
//            context->next_param = DEADLINE;
//            break;
//        case DEADLINE:
//            context->next_param = NONE;
//            break;
//        case NONE:
//            break;
//        default:
//            PRINTF("Param not supported\n");
//            msg->result = ETH_PLUGIN_RESULT_ERROR;
//            break;
//    }
//}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    uniswap_parameters_t *context = (uniswap_parameters_t *) msg->pluginContext;
    PRINTF("plugin provide parameter %d %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            handle_add_liquidity_eth(msg, context);
            break;
        case UNISWAP_DUMMY_2:
            //handle_dummy_two(msg, context);
            break;
        default:
            PRINTF("Selector Index %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}