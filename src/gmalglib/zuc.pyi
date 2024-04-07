ZUC_KEY_LENGTH: int
ZUC_IV_LENGTH: int
ZUC_WORD_LENGTH: int


class ZUC:
    """ZUC Algorithm."""

    def __init__(self, key: bytes, iv: bytes) -> None:
        """ZUC Algorithm.

        Args:
            key: Key.
            iv: Initial vector.
        """

    def generate(self) -> bytes:
        """Generate a word.

        Returns:
            word: 4 bytes word.
        """

    def copy(self) -> ZUC:
        """Copy a new ZUC object with identical state.

        Returns:
            zuc_obj: A new ZUC object.
        """
