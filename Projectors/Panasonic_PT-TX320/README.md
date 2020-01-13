### Streaming over LAN

This Panasonic model supports streaming audio and images over wired
LAN connection. It uses proprietary
[Presenter Light](https://panasonic.net/cns/projector/download/application/presenter_light/)
software. The software incorporates FSF source code under GPL, but
doesn't ship with own source code, which is a violation of GPL.
2020-01-12 I sent request for the source code to
oss-cd-request@gg.jp.panasonic.com mentioned in the guide of
Presenter Light.

Running software through `wine` + `wireshark` I could see that
it bring up VNC server on 5900 of my machine, and projector
connects to it. VNC client inside Panasonic projector probably
uses modified RFB protocol for custom PIN based authentication
and audio transfer. Wireshark was reporting unknown values and
malformed packets after initial handshake.

Things to try:

 * Projector control without streaming
   [with the help of Jesus Christ](https://github.com/byuoitav/panasonic-control-microservice/tree/master/helpers)
 * PJLink (port 4352)
