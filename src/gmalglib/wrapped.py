"""算法包装模块, 提供过程式调用方式."""

from . import sm2 as __sm2
from . import sm3 as __sm3
from . import sm4 as __sm4
from . import zuc as __zuc
import typing as __T


def sm3_digest(msg: bytes) -> bytes:
    """计算 SM3 哈希值."""

    return __sm3.SM3(msg).digest()


def sm3_kdf(sbytes: bytes, klen: int) -> bytes:
    """SM3 密钥派生函数."""

    return __sm3.SM3(sbytes).derive_key(klen)


def sm3_mac(msg: bytes, key: bytes) -> bytes:
    """SM3 消息认证码."""

    return __sm3.SM3(msg).mac(key)


def sm4_encrypt(key: bytes, block: bytes) -> bytes:
    """SM4 数据块加密."""

    return __sm4.SM4(key).encrypt(block)


def sm4_decrypt(key: bytes, block: bytes) -> bytes:
    """SM4 数据块解密."""

    return __sm4.SM4(key).decrypt(block)


def zuc_generate(key: bytes, iv: bytes, klen: int) -> bytes:
    """ZUC 伪随机密钥流生成."""

    words = []
    obj = __zuc.ZUC(key, iv)
    n, r = divmod(klen, __zuc.ZUC_WORD_LENGTH)
    for _ in range(n):
        words.append(obj.generate())
    if r > 0:
        words.append(obj.generate()[:r])
    return b"".join(words)


__sm2_pcmode = {"raw": __sm2.SM2_PCMODE_RAW, "compress": __sm2.SM2_PCMODE_COMPRESS, "mix": __sm2.SM2_PCMODE_MIX}


def sm2_is_sk_valid(sk: bytes) -> bool:
    """测试私钥是否合法."""

    return __sm2.SM2.is_sk_valid(sk)


def sm2_is_pk_valid(pk: bytes) -> bool:
    """测试公钥是否合法."""

    return __sm2.SM2.is_pk_valid(pk)


def sm2_is_keypair(sk: bytes, pk: bytes) -> bool:
    """检查是否是合法密钥对."""

    return __sm2.SM2.is_keypair(sk, pk)


def sm2_get_pk(sk: bytes, pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> bytes:
    """由私钥得到公钥."""

    return __sm2.SM2.get_pk(sk, __sm2_pcmode[pc_mode])


def sm2_convert_pk(pk: bytes, pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> bytes:
    """转换公钥格式."""

    return __sm2.SM2.convert_pk(pk, __sm2_pcmode[pc_mode])


def sm2_generate_keypair(pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> __T.Tuple[bytes, bytes]:
    """生成密钥对.

    Returns:
        sk: 私钥.
        pk: 公钥.
    """

    return __sm2.SM2(pc_mode=__sm2_pcmode[pc_mode]).generate_keypair()


def sm2_get_entity_info(pk: bytes, uid: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """获取实体信息."""

    return __sm2.SM2(pk=pk, uid=uid).get_entity_info()


def sm2_sign_digest(sk: bytes, digest: bytes, uid: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """对摘要进行签名."""

    return __sm2.SM2(sk=sk, uid=uid).sign_digest(digest)


def sm2_sign(sk: bytes, msg: bytes, uid: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """对消息进行签名."""

    return __sm2.SM2(sk=sk, uid=uid).sign(msg)


def sm2_verify_digest(pk: bytes, digest: bytes, uid: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """对摘要进行验签."""

    return __sm2.SM2(pk=pk, uid=uid).verify_digest(digest)


def sm2_verify(pk: bytes, msg: bytes, uid: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """对消息进行验签."""

    return __sm2.SM2(pk=pk, uid=uid).verify(msg)


def sm2_encrypt(pk: bytes, plain: bytes, pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> bytes:
    """加密数据."""

    return __sm2.SM2(pk=pk, pc_mode=__sm2_pcmode[pc_mode]).encrypt(plain)


def sm2_decrypt(sk: bytes, cipher: bytes) -> bytes:
    """解密数据."""

    return __sm2.SM2(sk=sk).decrypt(cipher)


def sm2_begin_key_exchange(sk: bytes, pc_mode: __T.Literal["raw", "compress", "mix"] = "raw") -> __T.Tuple[bytes, bytes]:
    """开始密钥交换.

    Returns:
        t: t 值.
        random_pt: 随机点 R.
    """

    return __sm2.SM2(sk, pc_mode=pc_mode).begin_key_exchange()


def sm2_end_key_exchange(pk: bytes, t: bytes, random_pt: bytes, pk_another: bytes, is_responder: bool, klen: int, uid: bytes = __sm2.SM2_DEFAULT_UID, uid_another: bytes = __sm2.SM2_DEFAULT_UID) -> bytes:
    """结束密钥交换.

    Args:
        pk: 自己的公钥.
        t: 从 begin_key_exchange 得到的 t 值.
        random_pt: 对方的随机点 R.
        pk_another: 对方的公钥.
        is_responder: 自己是否是响应方, 否则是发起方.
        klen: 要生成的密钥长度.
        uid: 自己的用户 ID.
        uid_another: 对方的用户 ID.

    Returns:
        key: 交换的密钥.
    """

    return __sm2.SM2(pk=pk, uid=uid).end_key_exchange(t, random_pt, pk_another, is_responder, klen, uid_another)
