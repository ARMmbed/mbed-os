Thread security fundamentals
============

## Authentication and key agreement

The fundamental security used in the Thread network is an elliptic curve variant of J-PAKE (EC-JPAKE), using the NIST P-256 elliptic curve. J-PAKE is a password-authenticated key exchange (PAKE) with _juggling_ (hence the J). It essentially uses elliptic curve Diffie-Hellmann for key agreement and Schnorr signatures as a Non-Interactive Zero-Knowledge (NIZK) proof mechanism to authenticate two peers and to establish a shared secret between them based on the passphrase. The author has published two internet drafts which describe J-PAKE [draft-hao-jpake-01](https://tools.ietf.org/html/draft-hao-jpake-01) and the Schnorr NIZK proof [draft-hao-schnorr-01](https://tools.ietf.org/html/draft-hao-schnorr-01).

A Transport Layer Security (TLS) 1.2 [RFC 5246](https://tools.ietf.org/html/rfc5246) handshake will be developed for EC-JPAKE, and it can potentially be used in both TLS and DTLS. Datagram Transport Layer Security (DTLS) [RFC 6347](https://tools.ietf.org/html/rfc6347) is a variant of TLS with additional fields in the records to make it suitable for use over an unreliable datagram-based transport (for example, User Datagram Protocol (UDP)), whereas TLS assumes a reliable transport such as Transport Control Protocol (TCP).

# Network-wide Key

The Thread Network is protected with a network-wide key, that is used at the Media Access Control (MAC) layer to protect the 802.15.4 MAC data frames. This is an elementary form of security used to prevent casual eavesdropping and targeted disruption of the Thread network from outsiders without knowledge of the network-wide key. As it is a network-wide key, compromise of any Thread device could potentially reveal the key; therefore, it is not typically used as the only form of protection within the Thread network. From the point of view of joining, it is used to discriminate between an authenticated and authorized Thread device and the joiner (in its initial state). The network-wide key, along with other network parameters, is delivered securely to a joiner using a Key Encryption Key (KEK) to secure it.

## Authentication

As a joiner is untrusted at the point of joining, it is common practice to enforce some sort of policing mechanism to ensure the joiner can be verified and at the same time limit the effect of rogue devices attempting to join the Thread network. In a Thread network, this requires the joiner to identify a Joiner Router and to communicate solely in a point-to-point fashion with the Joiner Router. The Joiner Router polices any traffic from the joiner and forwards it to the commissioner in a controlled manner to allow the authentication protocol (DTLS handshake) to execute.

In a case where the commissioner is not in direct communication with the joiner, the Joiner Router must relay the DTLS handshake with the commissioner. The Commissioning Relay Protocol provides encapsulation of the DTLS handshake and relaying of the DTLS handshake from the joiner all the way to the commissioner in a simple manner.

## Maintenance

A commissioner uses the Commissioning Protocol to keep a secure communication session alive and also to change certain parameters of the network, for example, the network name.
