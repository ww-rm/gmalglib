SM4_KEY_LENGTH: int
SM4_BLOCK_LENGTH: int


class SM4:
    def __init__(self, key: bytes) -> None:
        """SM4 Algorithm.

        Args:
            key: SM4 key.
        """

    def encrypt(self, block: bytes) -> bytes:
        """Encrypt.

        Args:
            block: Block data to be encrypted.

        Returns:
            cipher: Encrypted block data.
        """

    def decrypt(self, block: bytes) -> bytes:
        """Decrypt.

        Args:
            block: Block data to be decrypted.

        Returns:
            plain: Decrypted block data.
        """
