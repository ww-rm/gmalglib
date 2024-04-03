import secrets
import unittest

import gmalg

import gmalglib


class TestSM3(unittest.TestCase):
    def test_case1(self):
        sm31 = gmalg.SM3()
        sm32 = gmalglib.sm3.SM3()

        self.assertEqual(sm31.value(), sm32.digest())

        for i in range(64 * 4):
            sm31 = gmalg.SM3()
            sm32 = gmalglib.sm3.SM3()

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case2(self):
        sm31 = gmalg.SM3()
        sm32 = gmalglib.sm3.SM3()

        for i in range(64 * 4):
            sm31 = gmalg.SM3()
            sm32.reset()

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case3(self):
        sm31 = gmalg.SM3()
        sm32 = gmalglib.sm3.SM3()

        for i in range(64 * 4):

            msg = secrets.token_bytes(i)
            sm31.update(msg)
            sm32.update(msg)

            self.assertEqual(sm31.value(), sm32.digest())

    def test_case4(self):
        sm31 = gmalglib.sm3.SM3()
        sm32 = gmalglib.sm3.SM3()

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
        self.c = gmalglib.sm4.SM4(bytes.fromhex("0123456789ABCDEFFEDCBA9876543210"))

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
        self.assertRaises(ValueError, self.c.encrypt, b"123456781234567")
        self.assertRaises(ValueError, self.c.encrypt, b"12345678123456781")
        self.assertRaises(ValueError, self.c.decrypt, b"123456781234567")
        self.assertRaises(ValueError, self.c.decrypt, b"12345678123456781")


if __name__ == "__main__":
    unittest.main()
