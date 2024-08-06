FROM devkitpro/toolchain-base

#RUN curl -L https://codeberg.org/nezbednik/nez-wii/releases/download/nez-wii/nez-wii.db.tar.xz -o /tmp/nez-wii.db.tar.xz && \
#    tar -xf /tmp/nez-wii.db.tar.xz -C /opt/devkitpro/pacman/etc/pacman.d/ && \
#    rm /tmp/nez-wii.db.tar.xz

RUN dkp-pacman -Syyu --noconfirm gamecube-dev wii-dev wiiu-dev && \
    dkp-pacman -S --needed --noconfirm ppc-portlibs gamecube-portlibs wii-portlibs wiiu-portlibs && \
    dkp-pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo && \
#    dkp-pacman -S --needed --noconfirm ppc-zip ppc-jansson wii-winyl ppc-yuarel && \
    dkp-pacman -S --needed --noconfirm devkitARM && \
    yes | dkp-pacman -Scc

ENV DEVKITPPC=${DEVKITPRO}/devkitPPC
ENV DEVKITARM=/opt/devkitpro/devkitARM

RUN git clone https://github.com/GRRLIB/GRRLIB.git /depbuild/GRRLIB && \
    cd /depbuild/GRRLIB/GRRLIB && \
    make clean all install
