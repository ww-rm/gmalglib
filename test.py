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


if __name__ == "__main__":
    unittest.main()
