SM3_MAX_MSG_BITLEN: int
SM3_DIGEST_LENGTH: int


class SM3:
    """SM3 Algorithm."""

    def __init__(self, data: bytes = b"") -> None:
        """SM3 Algorithm.

        Args:
            data: Initial data.
        """

    def update(self, data: bytes) -> None:
        """Update state.

        Args:
            data: Bytes data.
        """

    def digest(self) -> bytes:
        """Get digest.

        Returns:
            digest: Digest in bytes.
        """

    def reset(self) -> None:
        """Reset internal state."""

    def copy(self) -> SM3:
        """Copy a new SM3 object with identical state.

        Returns:
            sm3_obj: A new SM3 object.
        """
