import "core-js/stable";
import "regenerator-runtime/runtime";
import Eth from "@ledgerhq/hw-app-eth";
import { byContractAddress } from "@ledgerhq/hw-app-eth/erc20";
import Zemu from "@zondax/zemu";
import { TransportStatusError } from "@ledgerhq/errors";
import { expect } from "../jest";

const { NANOS_ETH_ELF_PATH, NANOX_ETH_ELF_PATH, NANOS_UNISWAP_LIB, NANOX_LIDO_LIB, sim_options_nanos, sim_options_nanox, TIMEOUT } = require("generic.js");

const ORIGINAL_SNAPSHOT_PATH_PREFIX = "snapshots/add_liquidity/";
const SNAPSHOT_PATH_PREFIX = "snapshots/temp/";

const ORIGINAL_SNAPSHOT_PATH_NANOS = ORIGINAL_SNAPSHOT_PATH_PREFIX + "nanos/";
const ORIGINAL_SNAPSHOT_PATH_NANOX = ORIGINAL_SNAPSHOT_PATH_PREFIX + "nanox/";

const SNAPSHOT_PATH_NANOS = SNAPSHOT_PATH_PREFIX + "nanos/";
const SNAPSHOT_PATH_NANOX = SNAPSHOT_PATH_PREFIX + "nanox/";

test("Transfer nanos", async () => {
  jest.setTimeout(TIMEOUT);

  const sim = new Zemu(NANOS_ETH_ELF_PATH, NANOS_UNISWAP_LIB);

  try {
    console.log("DEBUG");
    await sim.start(sim_options_nanos);

    console.log("DEBUG");
    let transport = await sim.getTransport();
    const eth = new Eth(transport);

    let buffer = Buffer.from("3407556e69737761707a250d5630b4cf539739df2c5dacb4c659f2488de8e337009a9ee708c47d8ce568d272223277fabf7c6ef6ee", "hex");
    let tx = transport.send(0xe0, 0x12, 0x00, 0x00, buffer);
    buffer = Buffer.from("6703554e491f9840a85d5af5bf1d1762f925bdaddc4201f98400000012000000013045022100dad508227e3abec13a80691ee03bf64684cd87e1669e53c6b9c21403d742b4640220516824c46e3d424a7a2ba247bb0d1821754874a91c301996aa11c39321e597db", "hex");
    tx = transport.send(0xe0, 0x0a, 0x00, 0x00, buffer);
    buffer = Buffer.from("680457455448b4fbf271143f4fbf7b91a5ded31805e42b2208d600000012000000013045022100dad508227e3abec13a80691ee03bf64684cd87e1669e53c6b9c21403d742b4640220516824c46e3d424a7a2ba247bb0d1821754874a91c301996aa11c39321e597db", "hex");
    tx = transport.send(0xe0, 0x0a, 0x00, 0x00, buffer);
    buffer = Buffer.from("96058000002C8000003C800000020000000000000000F9012B0685174876E80283026BDB947A250D5630B4CF539739DF2C5DACB4C659F2488D80B90104E8E337000000000000000000000000001F9840A85D5AF5BF1D1762F925BDADDC4201F984000000000000000000000000B4FBF271143F4FBF7B91A5DED31805E42B2208D600000000000000000000000000000000000000000000", "hex");
    tx = transport.send(0xe0, 0x04, 0x00, 0x00, buffer);
    buffer = Buffer.from("9600000034EF7B208836DC000000000000000000000000000000000000000000000000000D9DB399B752DA0000000000000000000000000000000000000000000000000034ABB93B3FDA6C000000000000000000000000000000000000000000000000000D8C45E7070C090000000000000000000000005DD37A819253D52EC6C31F0323BD17C20C1B4CC2000000000000000000000000", "hex");
    tx = transport.send(0xe0, 0x04, 0x80, 0x00, buffer);
    buffer = Buffer.from("170000000000000000000000000000000060E4E885058080", "hex");
    tx = transport.send(0xe0, 0x04, 0x80, 0x00, buffer);



    // await eth.setExternalPlugin("ae7ab96520de3a18e5e111b5eaab095312d7fe84", "a1903eab");

    // Send transaction
    // let tx = transport.send(0xe0, 0x04, 0x00, 0x00, buffer);
    let filename;

    await sim.waitUntilScreenIsNot(sim.getMainMenuSnapshot());
    // Review tx
    filename = "tx_type_ui.png";
    await sim.snapshot(SNAPSHOT_PATH_NANOS + filename);
    const review = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    const expected_review = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(review).toEqual(expected_review);

    // Lido Stake message
    filename = "token_a_address.png";
    await sim.clickRight(SNAPSHOT_PATH_NANOS + filename);
    const lido = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    const expected_lido = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(lido).toEqual(expected_lido);

    // Stake 1/2 
    filename = "token_b_address.png";
    await sim.clickRight(SNAPSHOT_PATH_NANOS + filename);
    const stake_1 = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    const expected_stake_1 = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(stake_1).toEqual(expected_stake_1);

    // Stake 2/2
    filename = "benificiary.png";
    await sim.clickRight(SNAPSHOT_PATH_NANOS + filename);
    const stake_2 = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    const expected_stake_2 = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    expect(stake_2).toEqual(expected_stake_2);

    // Max Fees
    // filename = "fees.png";
    // await sim.clickRight(SNAPSHOT_PATH_NANOS + filename);
    // const fees = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    // const expected_fees = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    // expect(fees).toEqual(expected_fees);

    // Accept
    // filename = "accept.png";
    // await sim.clickRight(SNAPSHOT_PATH_NANOS + filename);
    // const accept = Zemu.LoadPng2RGB(SNAPSHOT_PATH_NANOS + filename);
    // const expected_accept = Zemu.LoadPng2RGB(ORIGINAL_SNAPSHOT_PATH_NANOS + filename);
    // expect(accept).toEqual(expected_accept);
    // 
    // await sim.clickBoth();
    // 
    // await expect(tx).resolves.toEqual(
    // Buffer.from([37, 58, 117, 33, 222, 177, 118, 205, 109, 64, 199, 254, 254, 98, 128, 162, 170, 45, 142, 50, 202, 51, 13, 224, 167, 106, 126, 52, 218, 62, 14, 93, 150, 92, 135, 129, 2, 6, 2, 22, 176, 87, 239, 126, 100, 158, 160, 167, 149, 110, 143, 61, 111, 95, 250, 186, 131, 168, 205, 180, 167, 149, 236, 53, 99, 144, 0])
    // );
  } finally {
    await sim.close();
  }
});
