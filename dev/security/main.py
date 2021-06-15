from Crypto.Hash.HMAC import HMAC

key = "key".encode()

res = HMAC(key, "pass".encode())
res = HMAC(key, res.digest() + "word".encode())

print(res.hexdigest())
print(HMAC(key, "password".encode()).hexdigest())