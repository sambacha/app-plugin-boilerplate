#include "uniswap_plugin.h"

static void handle_token_a_address(ethPluginProvideParameter_t *msg,
                                   uniswap_parameters_t *context) {
    memset(context->token_a_address, 0, sizeof(context->token_a_address));
    memcpy(context->token_a_address,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->token_a_address));

    PRINTF("TOKEN_A_ADDRESS CONTRACT: %.*H\n", ADDRESS_LENGTH, context->token_a_address);
}

static void handle_token_b_address(ethPluginProvideParameter_t *msg,
                                   uniswap_parameters_t *context) {
    memset(context->token_b_address, 0, sizeof(context->token_b_address));
    memcpy(context->token_b_address,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->token_b_address));
    PRINTF("GPIRIOU TOKEN_B_ADDRESS CONTRACT: %.*H\n", ADDRESS_LENGTH, context->token_b_address);
}

// Store the amount sent in the form of a string, without any ticker or
// decimals. These will be added when displaying.

// static void handle_eth_amount(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
//     memset(context->token_b_amount_sent, 0, sizeof(context->token_b_amount_sent));

//     // Convert to string.
//     amountToString(msg->parameter,
//                    PARAMETER_LENGTH,
//                    0,
//                    "",
//                    (char *) context->token_b_amount_sent,
//                    sizeof(context->token_b_amount_sent));
//     PRINTF("AMOUNT SENT: %s\n", context->token_b_amount_sent);
// }

static void handle_token_a_amount(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->token_a_amount_sent, 0, sizeof(context->token_a_amount_sent));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,   // No decimals
                   "",  // No ticker
                   (char *) context->token_a_amount_sent,
                   sizeof(context->token_a_amount_sent));

    PRINTF("TOKEN_A_ADDRESS A AMOUNT SENT: %s\n", context->token_a_amount_sent);
}

static void handle_token_b_amount(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->token_b_amount_sent, 0, sizeof(context->token_b_amount_sent));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,   // No decimals
                   "",  // No ticker
                   (char *) context->token_b_amount_sent,
                   sizeof(context->token_b_amount_sent));
    PRINTF("PENZO handle_token_b_amount() token_b_amount_sent: %s\n", context->token_b_amount_sent);
}

static void handle_beneficiary(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->beneficiary, 0, sizeof(context->beneficiary));
    memcpy(context->beneficiary,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_add_liquidity_eth(ethPluginProvideParameter_t *msg,
                                     uniswap_parameters_t *context) {
    // Describe ABI
    switch (context->next_param) {
        case TOKEN_A_ADDRESS:  // deposited token address
            handle_token_a_address(msg, context);
            context->next_param = AMOUNT_TOKEN_A;
            break;
        case AMOUNT_TOKEN_A:  // token amount deposited
            handle_token_a_amount(msg, context);
            PRINTF("after\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:  // not used
            PRINTF("token min\n");
            context->next_param = AMOUNT_ETH_MIN;
            break;
        case AMOUNT_ETH_MIN:
            // handle_eth_amount(msg, context); // this is useless as we don't use this variable
            PRINTF("eth min\n");
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // address receiving liquitity tokens
            PRINTF("benef\n");
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

static void handle_add_liquidity(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    // Describe ABI
    PRINTF("PENZO handle_add_liquidity\n");
    switch (context->next_param) {
        case TOKEN_A_ADDRESS:  // sent token address
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:  // sent token address
            handle_token_b_address(msg, context);
            context->next_param = AMOUNT_TOKEN_A;
            break;
        case AMOUNT_TOKEN_A:  // token amount sent
            handle_token_a_amount(msg, context);
            PRINTF("after\n");
            context->next_param = AMOUNT_TOKEN_B;
            break;
        case AMOUNT_TOKEN_B:  // token amount sent
            handle_token_b_amount(msg, context);
            PRINTF("after b\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:
            PRINTF("a min\n");
            context->next_param = AMOUNT_TOKEN_B_MIN;
            break;
        case AMOUNT_TOKEN_B_MIN:
            PRINTF("b min\n");
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // receiving address tokens
            PRINTF("benef\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
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

static void handle_remove_liquidity(ethPluginProvideParameter_t *msg,
                                    uniswap_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_A_ADDRESS:
            PRINTF("PENZO TOKEN_A_ADDRESS\n");
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:
            PRINTF("PENZO TOKEN_A_ADDRESS\n");
            handle_token_b_address(msg, context);
            context->next_param = LIQUIDITY;
            break;
        case LIQUIDITY:
            PRINTF("PENZO LIQUIDITY\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:
            PRINTF("PENZO TOKEN_A_MIN\n");
            handle_token_a_amount(msg, context);
            context->next_param = AMOUNT_TOKEN_B_MIN;
            break;
        case AMOUNT_TOKEN_B_MIN:
            PRINTF("PENZO ETH MIN\n");
            handle_token_b_amount(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("PENZO BENEFICIARY\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("PENZO DEALINE\n");
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

static void handle_remove_liquidity_eth(ethPluginProvideParameter_t *msg,
                                        uniswap_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_A_ADDRESS:
            PRINTF("PENZO TOKEN_A_ADDRESS\n");
            handle_token_a_address(msg, context);
            context->next_param = LIQUIDITY;
            break;
        case LIQUIDITY:
            PRINTF("PENZO LIQUIDITY\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:
            PRINTF("PENZO TOKEN_A_MIN\n");
            handle_token_a_amount(msg, context);
            context->next_param = AMOUNT_ETH_MIN;
            break;
        case AMOUNT_ETH_MIN:
            PRINTF("PENZO ETH MIN\n");
            handle_token_b_amount(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("PENZO BENEFICIARY\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("PENZO DEALINE\n");
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
            PRINTF("GPIRIOU ADD LIQUIDITY ETH\n");
            handle_add_liquidity_eth(msg, context);
            break;
        case ADD_LIQUIDITY:
            PRINTF("GPIRIOU LIQUIDITY\n");
            handle_add_liquidity(msg, context);
            break;
        case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT_FEE:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
        case REMOVE_LIQUIDITY_ETH_FEE:
            PRINTF("GPIRIOU REMOVE LIQUIDITY ETH\n");
            handle_remove_liquidity_eth(msg, context);
            break;
        case REMOVE_LIQUIDITY:
            handle_remove_liquidity(msg, context);
            break;
        default:
            PRINTF("Selector Index %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}