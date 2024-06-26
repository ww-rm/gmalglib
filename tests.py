import secrets
import unittest

import gmalg as alg

import gmalglib as lib


class TestSM3(unittest.TestCase):
    def test_case1(self):
        sm31 = alg.SM3()
        sm32 = lib.sm3.SM3()

        self.assertEqual(sm31.value(), sm32.digest())

        for i in range(64 * 4):
            sm31 = alg.SM3()
            sm32 = lib.sm3.SM3()

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case2(self):
        sm31 = alg.SM3()
        sm32 = lib.sm3.SM3()

        for i in range(64 * 4):
            sm31 = alg.SM3()
            sm32.reset()

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case3(self):
        sm31 = alg.SM3()
        sm32 = lib.sm3.SM3()

        for i in range(64 * 4):

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case4(self):
        sm31 = lib.sm3.SM3()
        sm32 = lib.sm3.SM3()

        for i in range(64 * 4):
            msg = secrets.token_bytes(i)
            if i % 2:
                sm31.update(msg)
                sm32 = sm31.copy()
            else:
                sm32.update(msg)
                sm31 = sm32.copy()

            self.assertEqual(sm31.digest(), sm32.digest())


class TestSM4(unittest.TestCase):
    def setUp(self) -> None:
        self.c = lib.sm4.SM4(bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))

    def test_case1(self):
        cipher = self.c.encrypt(bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))
        self.assertEqual(cipher, bytes.fromhex("681edf34d206965e86b3e94f536e4246"))

        plain = self.c.decrypt(cipher)
        self.assertEqual(plain, bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))

    def test_case2(self):
        cipher = self.c.encrypt(bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))
        for _ in range(999999):
            cipher = self.c.encrypt(cipher)
        self.assertEqual(cipher, bytes.fromhex("595298c7c6fd271f0402f804c33d3f66"))

        plain = self.c.decrypt(cipher)
        for _ in range(999999):
            plain = self.c.decrypt(plain)
        self.assertEqual(plain, bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))

    def test_raises(self):
        self.assertRaises(ValueError, lib.sm4.SM4, b"123456781234567")
        self.assertRaises(ValueError, lib.sm4.SM4, b"12345678123456781")
        self.assertRaises(ValueError, self.c.encrypt, b"123456781234567")
        self.assertRaises(ValueError, self.c.encrypt, b"12345678123456781")
        self.assertRaises(ValueError, self.c.decrypt, b"123456781234567")
        self.assertRaises(ValueError, self.c.decrypt, b"12345678123456781")


class TestZUC(unittest.TestCase):
    def test_case1(self):
        z = lib.zuc.ZUC(bytes.fromhex("00000000000000000000000000000000"), bytes.fromhex("00000000000000000000000000000000"))
        self.assertEqual(z.generate(), bytes.fromhex("27bede74"))
        self.assertEqual(z.generate(), bytes.fromhex("018082da"))

    def test_case2(self):
        z = lib.zuc.ZUC(bytes.fromhex("ffffffffffffffffffffffffffffffff"), bytes.fromhex("ffffffffffffffffffffffffffffffff"))
        self.assertEqual(z.generate(), bytes.fromhex("0657cfa0"))
        self.assertEqual(z.generate(), bytes.fromhex("7096398b"))

    def test_case3(self):
        z = lib.zuc.ZUC(bytes.fromhex("3d4c4be96a82fdaeb58f641db17b455b"), bytes.fromhex("84319aa8de6915ca1f6bda6bfbd8c766"))
        self.assertEqual(z.generate(), bytes.fromhex("14f1c272"))
        self.assertEqual(z.generate(), bytes.fromhex("3279c419"))

    def test_case4(self):
        self.assertRaises(ValueError, lib.zuc.ZUC, b"123456781234567", b"123456781234567")
        self.assertRaises(ValueError, lib.zuc.ZUC, b"1234567812345678", b"12345678123456781")


class TestSM2(unittest.TestCase):
    def test_key(self):
        d, _ = alg.SM2().generate_keypair()
        d = b"\x00" * (32 - len(d)) + d
        self.assertEqual(alg.SM2().generate_pk(d), lib.sm2.SM2.get_pk(d))

        d, _ = lib.sm2.SM2().generate_keypair()
        d = b"\x00" * (32 - len(d)) + d
        self.assertEqual(alg.SM2().generate_pk(d), lib.sm2.SM2.get_pk(d))

        d, _ = alg.SM2().generate_keypair()
        d = b"\x00" * (32 - len(d)) + d
        self.assertEqual(alg.SM2(pc_mode=alg.PC_MODE.COMPRESS).generate_pk(d), lib.sm2.SM2.get_pk(d, lib.sm2.SM2_PCMODE_COMPRESS))

        d, _ = lib.sm2.SM2().generate_keypair()
        d = b"\x00" * (32 - len(d)) + d
        self.assertEqual(alg.SM2(pc_mode=alg.PC_MODE.MIXED).generate_pk(d), lib.sm2.SM2.get_pk(d, lib.sm2.SM2_PCMODE_MIX))

        d, pk = lib.sm2.SM2().generate_keypair()
        pk2 = lib.sm2.SM2.get_pk(d, lib.sm2.SM2_PCMODE_COMPRESS)
        pk2 = lib.sm2.SM2.convert_pk(pk2, lib.sm2.SM2_PCMODE_RAW)

        self.assertEqual(pk, pk2)
        self.assertTrue(lib.sm2.SM2.is_keypair(d, pk))
        pk2 = lib.sm2.SM2.convert_pk(pk, lib.sm2.SM2_PCMODE_COMPRESS)
        self.assertTrue(lib.sm2.SM2.is_keypair(d, pk2))

    def test_sign1(self):
        sm2 = lib.sm2.SM2(
            bytes.fromhex("3945208F 7B2144B1 3F36E38A C6D39F95 88939369 2860B51A 42FB81EF 4DF7C5B8"),
            bytes.fromhex("04"
                          "09F9DF31 1E5421A1 50DD7D16 1E4BC5C6 72179FAD 1833FC07 6BB08FF3 56F35020"
                          "CCEA490C E26775A5 2DC6EA71 8CC1AA60 0AED05FB F35E084A 6632F607 2DA9AD13"),
            rnd_fn=lambda _: 0x59276E27_D506861A_16680F3A_D9C02DCC_EF3CC1FA_3CDBE4CE_6D54B80D_EAC1BC21.to_bytes(32, "little")
        )

        signature = sm2.sign(b"message digest")
        self.assertEqual(signature, bytes.fromhex("F5A03B06 48D2C463 0EEAC513 E1BB81A1 5944DA38 27D5B741 43AC7EAC EEE720B3"
                                                  "B1B6AA29 DF212FD8 763182BC 0D421CA1 BB9038FD 1F7F42D4 840B69C4 85BBC1AA"))

        self.assertEqual(sm2.verify(b"message digest", signature), True)

    def test_sign2(self):
        d, pk = lib.sm2.SM2().generate_keypair()
        sm2 = lib.sm2.SM2(d, pk, b"test")

        plain = b"random SM2 sign test random SM2 sign test random SM2 sign test random SM2 sign test random SM2 sign test random SM2 sign test random SM2 sign test random SM2 sign test"
        signature = sm2.sign(plain)
        self.assertEqual(sm2.verify(plain, signature), True)

    def test_sign3(self):
        d, pk = lib.sm2.SM2().generate_keypair()
        sm2 = lib.sm2.SM2(d, pk, b"test")

        plain = b"12345678123456781234567812345678"
        digest = sm2.sign_digest(plain)
        self.assertEqual(sm2.verify_digest(plain, digest), True)

    def test_sign4(self):
        sk, pk = alg.SM2().generate_keypair()
        sk = b"\x00" * (32 - len(sk)) + sk

        algsm2 = alg.SM2(sk, b"test_sign4", pk)
        libsm2 = lib.sm2.SM2(sk, pk, b"test_sign4")

        msg = b"12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678"
        for _ in range(10):
            r, s = algsm2.sign(msg)
            r = b"\x00" * (32 - len(r)) + r
            s = b"\x00" * (32 - len(s)) + s
            self.assertTrue(libsm2.verify(msg, r + s))

        for _ in range(10):
            s = libsm2.sign(msg)
            self.assertTrue(algsm2.verify(msg, s[:32], s[32:]))

    def test_sign5(self):
        sk, pk = lib.sm2.SM2().generate_keypair()
        libsm2 = lib.sm2.SM2(sk, pk, b"test_sign5")
        msg = b"fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!fast sign!"

        for _ in range(10):
            self.assertTrue(libsm2.verify(msg, libsm2.sign(msg)))

    def test_encrypt1(self):
        sm2 = lib.sm2.SM2(
            bytes.fromhex("3945208F 7B2144B1 3F36E38A C6D39F95 88939369 2860B51A 42FB81EF 4DF7C5B8"),
            bytes.fromhex("04"
                          "09F9DF31 1E5421A1 50DD7D16 1E4BC5C6 72179FAD 1833FC07 6BB08FF3 56F35020"
                          "CCEA490C E26775A5 2DC6EA71 8CC1AA60 0AED05FB F35E084A 6632F607 2DA9AD13"),
            rnd_fn=lambda _: 0x59276E27_D506861A_16680F3A_D9C02DCC_EF3CC1FA_3CDBE4CE_6D54B80D_EAC1BC21.to_bytes(32, "little"),
        )

        cipher = sm2.encrypt(b"encryption standard")
        self.assertEqual(cipher, bytes.fromhex("04"
                                               "04EBFC71 8E8D1798 62043226 8E77FEB6 415E2EDE 0E073C0F 4F640ECD 2E149A73"
                                               "E858F9D8 1E5430A5 7B36DAAB 8F950A3C 64E6EE6A 63094D99 283AFF76 7E124DF0"
                                               "59983C18 F809E262 923C53AE C295D303 83B54E39 D609D160 AFCB1908 D0BD8766"
                                               "21886C A989CA9C 7D580873 07CA9309 2D651EFA"))

        self.assertEqual(sm2.decrypt(cipher), b"encryption standard")

    def test_encrypt2(self):
        d, pk = lib.sm2.SM2().generate_keypair()
        plain = b"random SM2 encrypt test random SM2 encrypt test random SM2 encrypt test random SM2 encrypt test random SM2 encrypt test random SM2 encrypt test random SM2 encrypt test"

        sm2 = lib.sm2.SM2(d, pk=pk)
        cipher = sm2.encrypt(plain)
        self.assertEqual(sm2.decrypt(cipher), plain)

        sm2 = lib.sm2.SM2(d, pk=pk, pc_mode=lib.sm2.SM2_PCMODE_COMPRESS)
        cipher = sm2.encrypt(plain)
        self.assertEqual(sm2.decrypt(cipher), plain)

        sm2 = lib.sm2.SM2(d, pk=pk, pc_mode=lib.sm2.SM2_PCMODE_MIX)
        cipher = sm2.encrypt(plain)
        self.assertEqual(sm2.decrypt(cipher), plain)

    def test_encrypt3(self):
        sk, pk = alg.SM2().generate_keypair()
        sk = b"\x00" * (32 - len(sk)) + sk

        algsm2 = alg.SM2(sk, b"test_encrypt3", pk, pc_mode=alg.PC_MODE.RAW)
        libsm2 = lib.sm2.SM2(sk, pk, b"test_encrypt3", pc_mode=lib.sm2.SM2_PCMODE_COMPRESS)

        msg = b"12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678"
        for _ in range(10):
            cipher = algsm2.encrypt(msg)
            self.assertEqual(libsm2.decrypt(cipher), msg)

        for _ in range(10):
            cipher = libsm2.encrypt(msg)
            self.assertTrue(algsm2.decrypt(cipher), msg)

    def test_encrypt4(self):
        sk, pk = lib.sm2.SM2().generate_keypair()
        libsm2 = lib.sm2.SM2(sk, pk, b"test_encrypt4")
        msg = b"fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!fast encrypt!"

        for _ in range(10):
            self.assertTrue(libsm2.decrypt(libsm2.encrypt(msg)))

    def test_keyxchg(self):
        PA = bytes.fromhex("04"
                           "160E1289 7DF4EDB6 1DD812FE B96748FB D3CCF4FF E26AA6F6 DB9540AF 49C94232"
                           "4A7DAD08 BB9A4595 31694BEB 20AA489D 6649975E 1BFCF8C4 741B78B4 B223007F")
        sm2A = lib.sm2.SM2(
            bytes.fromhex("81EB26E9 41BB5AF1 6DF11649 5F906952 72AE2CD6 3D6C4AE1 678418BE 48230029"),
            PA,
            rnd_fn=lambda _: 0xD4DE1547_4DB74D06_491C440D_305E0124_00990F3E_390C7E87_153C12DB_2EA60BB3.to_bytes(32, "little")
        )

        PB = bytes.fromhex("04"
                           "6AE848C5 7C53C7B1 B5FA99EB 2286AF07 8BA64C64 591B8B56 6F7357D5 76F16DFB"
                           "EE489D77 1621A27B 36C5C799 2062E9CD 09A92643 86F3FBEA 54DFF693 05621C4D")
        sm2B = lib.sm2.SM2(
            bytes.fromhex("78512991 7D45A9EA 5437A593 56B82338 EAADDA6C EB199088 F14AE10D EFA229B5"),
            PB,
            rnd_fn=lambda _: 0x7E071248_14B30948_9125EAED_10111316_4EBF0F34_58C5BD88_335C1F9D_596243D6.to_bytes(32, "little")
        )

        tA, RA = sm2A.begin_key_exchange()
        tB, RB = sm2B.begin_key_exchange()

        KB = sm2B.end_key_exchange(tB, RA, PA, True, 16)
        KA = sm2A.end_key_exchange(tA, RB, PB, False, 16)

        self.assertEqual(KA, KB)
        self.assertEqual(KA, bytes.fromhex("6C893473 54DE2484 C60B4AB1 FDE4C6E5"))

    def test_keyxchg2(self):
        for _ in range(10):
            skA, pkA = lib.sm2.SM2().generate_keypair()
            skB, pkB = lib.sm2.SM2().generate_keypair()

            sm2A = lib.sm2.SM2(skA, uid=b"Alice", pc_mode=lib.sm2.SM2_PCMODE_COMPRESS)
            sm2B = lib.sm2.SM2(skB, uid=b"Bob")

            tA, RA = sm2A.begin_key_exchange()
            tB, RB = sm2B.begin_key_exchange()

            KB = sm2B.end_key_exchange(tB, RA, pkA, True, 17, b"Alice")
            KA = sm2A.end_key_exchange(tA, RB, pkB, False, 17, b"Bob")

            self.assertEqual(KA, KB)

    def test_keyxchg3(self):
        for _ in range(10):
            skA, pkA = lib.sm2.SM2().generate_keypair()
            skB, pkB = lib.sm2.SM2().generate_keypair()

            sm2A = lib.sm2.SM2(skA, uid=b"Alice", pc_mode=lib.sm2.SM2_PCMODE_COMPRESS)
            sm2B = alg.sm2.SM2(skB, uid=b"Bob")

            tA, RA = sm2A.begin_key_exchange()
            RB, tB = sm2B.begin_key_exchange()

            KB = sm2B.end_key_exchange(17, tB, RA, b"Alice", pkA, alg.KEYXCHG_MODE.RESPONDER)
            KA = sm2A.end_key_exchange(tA, RB, pkB, False, 17, b"Bob")

            self.assertEqual(KA, KB)


if __name__ == "__main__":
    unittest.main()
