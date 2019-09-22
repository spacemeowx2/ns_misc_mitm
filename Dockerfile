FROM devkitpro/devkita64_devkitarm:20190212

RUN dkp-pacman -Sy --noconfirm devkitA64
RUN git clone -b v2.5.0 https://github.com/switchbrew/libnx.git --depth=1 \
    && cd libnx \
    && make install \
    && cd .. \
    && rm -rf libnx

VOLUME [ "/code" ]
WORKDIR /code

CMD [ "make", "-j8" ]
