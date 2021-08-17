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

static void handle_token_a_amount(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->token_a_amount_sent, 0, sizeof(context->token_a_amount_sent));
    memcpy(context->token_a_amount_sent, msg->parameter, sizeof(context->token_a_amount_sent));
    PRINTF("GPIRIOU TEST TOKEN A AMOUNT:\n");
    print_bytes(context->token_a_amount_sent, sizeof(context->token_a_amount_sent));
}

static void handle_token_b_amount(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    memset(context->token_b_amount_sent, 0, sizeof(context->token_b_amount_sent));
    memcpy(context->token_b_amount_sent, msg->parameter, sizeof(context->token_b_amount_sent));
    PRINTF("GPIRIOU PRINT PARAMETER:\n");
    print_bytes(msg->parameter, PARAMETER_LENGTH);
    PRINTF("GPIRIOU TEST TOKEN B AMOUNT:\n");
    print_bytes(context->token_b_amount_sent, sizeof(context->token_b_amount_sent));
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
    PRINTF("GPIRIOU handle_add_liquidity eth\n");
    switch (context->next_param) {
        case TOKEN_B_ADDRESS:  // Token A is ETH
            handle_token_b_address(msg, context);
            context->next_param = AMOUNT_TOKEN_B;
            break;
        case AMOUNT_TOKEN_B:  // token amount deposited
            handle_token_b_amount(msg, context);
            PRINTF("after\n");
            context->next_param = AMOUNT_TOKEN_B_MIN;
            break;
        case AMOUNT_TOKEN_B_MIN:  // not used       !! AMOUNT TOKEN A ????
            PRINTF("token min\n");
            context->next_param = AMOUNT_ETH_MIN;
            break;
        case AMOUNT_ETH_MIN:  // not used
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
    PRINTF("GPIRIOU handle_add_liquidity\n");
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
            PRINTF("GPIRIOU HANDLE TOKEN A AMOUNT\n");
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
            PRINTF("GPIRIOU TOKEN_A_ADDRESS\n");
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:
            PRINTF("GPIRIOU TOKEN_A_ADDRESS\n");
            handle_token_b_address(msg, context);
            context->next_param = LIQUIDITY;
            break;
        case LIQUIDITY:
            PRINTF("GPIRIOU LIQUIDITY\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:
            PRINTF("GPIRIOU TOKEN_A_MIN\n");
            handle_token_a_amount(msg, context);
            context->next_param = AMOUNT_TOKEN_B_MIN;
            break;
        case AMOUNT_TOKEN_B_MIN:
            PRINTF("GPIRIOU ETH MIN\n");
            handle_token_b_amount(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("GPIRIOU BENEFICIARY\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("GPIRIOU DEALINE\n");
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
            PRINTF("GPIRIOU TOKEN_A_ADDRESS\n");
            handle_token_a_address(msg, context);
            context->next_param = LIQUIDITY;
            break;
        case LIQUIDITY:
            PRINTF("GPIRIOU LIQUIDITY\n");
            context->next_param = AMOUNT_TOKEN_A_MIN;
            break;
        case AMOUNT_TOKEN_A_MIN:
            PRINTF("GPIRIOU TOKEN_A_MIN\n");
            handle_token_a_amount(msg, context);
            context->next_param = AMOUNT_ETH_MIN;
            break;
        case AMOUNT_ETH_MIN:
            PRINTF("GPIRIOU ETH MIN\n");
            handle_token_b_amount(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("GPIRIOU BENEFICIARY\n");
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("GPIRIOU DEALINE\n");
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

static void handle_swap_eth(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    PRINTF("GPIRIOU msg->offset: %d\n", msg->parameterOffset);
    PRINTF("GPIRIOU next_param: %d, skip: %d\n",
           context->next_param == TOKEN_B_ADDRESS,
           context->skip);

    // skip through the path array to the last index.
    if (context->next_param == TOKEN_B_ADDRESS && context->skip) {
        PRINTF("GPIRIOU SKIPPING\n");
        context->skip--;
        return;
    }
    // msg->parameter is pointing at the previously found offset.
    if ((context->path_offset) && msg->parameterOffset == context->path_offset) {
        PRINTF("GPIRIOU PATH LENGTH: %u\n", msg->parameter[PARAMETER_LENGTH - 1] - 1);
        // -2 because (last array index + we already skip 1 by returning here)
        context->skip = msg->parameter[PARAMETER_LENGTH - 1] - 2;
        context->next_param = TOKEN_A_ADDRESS;
        return;
    }
    switch (context->next_param) {
        case AMOUNT_OUT:
            // save initial parameter offset
            // GPIRIOU should be U2BE ?
            // context->path_offset = msg->parameterOffset;
            PRINTF("CURRENT PARAM: AMOUNT_IN INIT\n");
            handle_token_b_amount(msg, context);  // Token A is ETH
            PRINTF("AMOUNT OUT: %s\n", context->token_b_amount_sent);
            context->next_param = PATH_OFFSET;
            break;
        case PATH_OFFSET:
            PRINTF("CURRENT PARAM: PATH INIT\n");
            PRINTF("GPIRIOU PATH TEST: %d\n", msg->parameter[PARAMETER_LENGTH - 1]);
            PRINTF("PATH offset: %d\n", U2BE(msg->parameter, PARAMETER_LENGTH - 2));
            // context->path_offset += U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            // or
            context->path_offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2) + SELECTOR_SIZE;
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("CURRENT PARAM: BENEFICIARY INIT\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("CURRENT PARAM: DEADLINE INIT\n");
            context->next_param = NONE;
            break;
        case TOKEN_A_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_A_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_B_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_b_address(msg, context);
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

static void handle_swap_exact_tokens(ethPluginProvideParameter_t *msg,
                                     uniswap_parameters_t *context) {
    PRINTF("GPIRIOU msg->offset: %d\n", msg->parameterOffset);
    PRINTF("GPIRIOU next_param: %d, skip: %d\n",
           context->next_param == TOKEN_B_ADDRESS,
           context->skip);

    // skip through the path array to the last index.
    if (context->next_param == TOKEN_B_ADDRESS && context->skip) {
        PRINTF("GPIRIOU SKIPPING\n");
        context->skip--;
        return;
    }
    // msg->parameter is pointing at the previously found offset.
    if ((context->path_offset) && msg->parameterOffset == context->path_offset) {
        PRINTF("GPIRIOU PATH LENGTH: %u\n", msg->parameter[PARAMETER_LENGTH - 1] - 1);
        // -2 because (last array index + we already skip 1 by returning here)
        context->skip = msg->parameter[PARAMETER_LENGTH - 1] - 2;
        context->next_param = TOKEN_A_ADDRESS;
        return;
    }
    switch (context->next_param) {
        case AMOUNT_IN:
            // save initial parameter offset
            // GPIRIOU should be U2BE ?
            // context->path_offset = msg->parameterOffset;
            PRINTF("CURRENT PARAM: AMOUNT_IN INIT\n");
            handle_token_a_amount(msg, context);
            PRINTF("AMOUNT OUT: %s\n", context->token_a_amount_sent);
            context->next_param = AMOUNT_OUT;
            break;
        case AMOUNT_OUT:
            PRINTF("CURRENT PARAM: AMOUNT_IN_MAX INIT\n");
            handle_token_b_amount(msg, context);
            PRINTF("GPIRIOU AMOUNT IN MAX: %s\n", context->token_b_amount_sent);
            context->next_param = PATH_OFFSET;
            break;
        case PATH_OFFSET:
            PRINTF("CURRENT PARAM: PATH INIT\n");
            PRINTF("GPIRIOU PATH TEST: %d\n", msg->parameter[PARAMETER_LENGTH - 1]);
            PRINTF("PATH offset: %d\n", U2BE(msg->parameter, PARAMETER_LENGTH - 2));
            // context->path_offset += U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            // or
            context->path_offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2) + SELECTOR_SIZE;
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("CURRENT PARAM: BENEFICIARY INIT\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("CURRENT PARAM: DEADLINE INIT\n");
            context->next_param = NONE;
            break;
        case TOKEN_A_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_A_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_B_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_b_address(msg, context);
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

static void handle_swap_tokens(ethPluginProvideParameter_t *msg, uniswap_parameters_t *context) {
    PRINTF("GPIRIOU msg->offset: %d\n", msg->parameterOffset);
    PRINTF("GPIRIOU next_param: %d, skip: %d\n",
           context->next_param == TOKEN_B_ADDRESS,
           context->skip);

    // skip through the path array to the last index.
    if (context->next_param == TOKEN_B_ADDRESS && context->skip) {
        PRINTF("GPIRIOU SKIPPING\n");
        context->skip--;
        return;
    }
    // msg->parameter is pointing at the previously found offset.
    if ((context->path_offset) && msg->parameterOffset == context->path_offset) {
        PRINTF("GPIRIOU PATH LENGTH: %u\n", msg->parameter[PARAMETER_LENGTH - 1] - 1);
        // -2 because (last array index + we already skip 1 by returning here)
        context->skip = msg->parameter[PARAMETER_LENGTH - 1] - 2;
        context->next_param = TOKEN_A_ADDRESS;
        return;
    }
    switch (context->next_param) {
        case AMOUNT_OUT:
            // save initial parameter offset
            // GPIRIOU should be U2BE ?
            // context->path_offset = msg->parameterOffset;
            PRINTF("CURRENT PARAM: AMOUNT_OUT INIT\n");
            handle_token_b_amount(msg, context);
            PRINTF("AMOUNT OUT: %s\n", context->token_b_amount_sent);
            context->next_param = AMOUNT_IN_MAX;
            break;
        case AMOUNT_IN_MAX:
            PRINTF("CURRENT PARAM: AMOUNT_IN_MAX INIT\n");
            handle_token_b_amount(msg, context);
            PRINTF("GPIRIOU AMOUNT IN MAX: %s\n", context->token_a_amount_sent);
            context->next_param = PATH_OFFSET;
            break;
        case PATH_OFFSET:
            PRINTF("CURRENT PARAM: PATH INIT\n");
            PRINTF("GPIRIOU PATH TEST: %d\n", msg->parameter[PARAMETER_LENGTH - 1]);
            PRINTF("PATH offset: %d\n", U2BE(msg->parameter, PARAMETER_LENGTH - 2));
            // context->path_offset += U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            // or
            context->path_offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2) + SELECTOR_SIZE;
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:
            PRINTF("CURRENT PARAM: BENEFICIARY INIT\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_beneficiary(msg, context);
            context->next_param = DEADLINE;
            break;
        case DEADLINE:
            PRINTF("CURRENT PARAM: DEADLINE INIT\n");
            context->next_param = NONE;
            break;
        case TOKEN_A_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_A_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_a_address(msg, context);
            context->next_param = TOKEN_B_ADDRESS;
            break;
        case TOKEN_B_ADDRESS:
            PRINTF("CURRENT PARAM: TOKEN_B_ADDRESS\n");
            print_bytes(&msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH], ADDRESS_LENGTH);
            handle_token_b_address(msg, context);
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
    PRINTF("PROVIDE PARAMETER, selector: %d\n", context->selectorIndex);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case ADD_LIQUIDITY_ETH:
            handle_add_liquidity_eth(msg, context);
            break;
        case ADD_LIQUIDITY:
            handle_add_liquidity(msg, context);
            break;
        case REMOVE_LIQUIDITY_ETH:
        case REMOVE_LIQUIDITY_ETH_PERMIT_FEE:
        case REMOVE_LIQUIDITY_ETH_PERMIT:
        case REMOVE_LIQUIDITY_ETH_FEE:
            handle_remove_liquidity_eth(msg, context);
            break;
        case REMOVE_LIQUIDITY:
        case REMOVE_LIQUIDITY_PERMIT:
            handle_remove_liquidity(msg, context);
            break;
        case SWAP_EXACT_ETH_FOR_TOKENS:
        case SWAP_EXACT_ETH_FOR_TOKENS_FEE:
            handle_swap_eth(msg, context);
            break;
        case SWAP_EXACT_TOKENS_FOR_ETH:
        case SWAP_EXACT_TOKENS_FOR_ETH_FEE:
        case SWAP_EXACT_TOKENS_FOR_TOKENS:
        case SWAP_EXACT_TOKENS_FOR_TOKENS_FEE:
            handle_swap_exact_tokens(msg, context);
            break;
        case SWAP_TOKENS_FOR_EXACT_ETH:
        case SWAP_TOKENS_FOR_EXACT_TOKENS:
            handle_swap_tokens(msg, context);
            break;
        default:
            PRINTF("Selector Index %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}