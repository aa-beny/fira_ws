FROM nvidia/cuda:11.7.1-cudnn8-devel-ubuntu22.04

#FROM nvidia/cuda:11.8.0-cudnn8-devel-ubuntu22.04
############################## SYSTEM PARAMETERS ##############################
ARG USER=developer
ARG GROUP=developer
ARG UID=1000
ARG GID=${UID}
ARG SHELL=/bin/bash

ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=all
ENV TZ=Asia/Taipei
ENV DEBIAN_FRONTEND=noninteractive

############################## USER SETUP ####################################
RUN groupadd --gid ${GID} ${GROUP} && \
    useradd --gid ${GID} --uid ${UID} -ms ${SHELL} ${USER} && \
    mkdir -p /etc/sudoers.d && \
    echo "${USER} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/${USER} && \
    chmod 0440 /etc/sudoers.d/${USER} && \
    ln -snf /usr/share/zoneinfo/${TZ} /etc/localtime && \
    echo ${TZ} > /etc/timezone && \
    apt-get update && apt-get install -y --no-install-recommends \
    locales tzdata && \
    locale-gen zh_TW.UTF-8 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

ENV LANG=zh_TW.UTF-8
ENV LC_ALL=zh_TW.UTF-8

# 切換回官方鏡像源
RUN sed -i 's@ftp.tku.edu.tw@archive.ubuntu.com@g' /etc/apt/sources.list

############################## INSTALL ROS2 HUMBLE ############################
RUN apt-get update && apt-get install -y curl gnupg2 lsb-release ca-certificates && \
    curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /tmp/ros.key && \
    gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg /tmp/ros.key && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" > /etc/apt/sources.list.d/ros2.list && \
    rm /tmp/ros.key && apt-get update && \
    apt-get install -y --no-install-recommends \
    ros-humble-desktop-full ros-humble-rmw-cyclonedds-cpp python3-colcon-common-extensions

############################## BASIC TOOLS & LIBS #############################
RUN apt-get update && apt-get install -y --no-install-recommends \
    sudo git wget curl tmux terminator htop psmisc udev libtool \
    python3-pip python3-dev python3-setuptools python3-distutils \
    build-essential software-properties-common \
    libgl1-mesa-glx libglib2.0-0 g++ cmake pkg-config \
    libboost-serialization-dev libboost-filesystem-dev libboost-system-dev \
    libboost-program-options-dev libboost-test-dev libeigen3-dev libode-dev \
    libyaml-cpp-dev castxml libboost-python-dev libboost-numpy-dev \
    python3-numpy pypy3 python3-pyqt5.qtopengl freeglut3-dev libassimp-dev \
    python3-opengl python3-flask python3-celery libccd-dev libfcl-dev \
    python-is-python3 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

############################## INSTALL RealSense SDK ##########################
RUN apt-get update && apt-get install -y libssl-dev libusb-1.0-0-dev \
    libgtk-3-dev libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libudev-dev
RUN pip install pyrealsense2

WORKDIR /opt
RUN git clone https://github.com/IntelRealSense/librealsense.git && \
    cd librealsense && git checkout v2.54.2 && \
    cp config/99-realsense-libusb.rules /etc/udev/rules.d/ && \
    mkdir build && cd build && \
    cmake ../ -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_EXAMPLES=false \
              -DBUILD_GRAPHICAL_EXAMPLES=false \
              -DBUILD_PYTHON_BINDINGS=bool:true \
              -DPYTHON_EXECUTABLE=/usr/bin/python3 && \
    make -j$(nproc) && make install && ldconfig

############################## INSTALL rosdep & vcs ###########################
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-rosdep python3-vcstool && \
    rosdep update || true

############################## pyrealsense2 Python Link ########################
RUN PYTHON_VERSION=$(python3 -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')") && \
    PYTHON_SITE_PACKAGES=$(python3 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())") && \
    ln -sf /usr/local/lib/python${PYTHON_VERSION}/site-packages/pyrealsense2*.so ${PYTHON_SITE_PACKAGES}/

############################## INSTALL PaddleOCR (GPU) ########################
RUN pip3 install --upgrade pip && \
    pip3 install numpy==1.26.4 && \
    pip3 install paddlepaddle-gpu==2.4.2.post117 -f https://www.paddlepaddle.org.cn/whl/linux/mkl/avx/stable.html && \
    pip3 install "paddleocr>=2.0.1"

############################## PyGCCXML & PyOpenGL ############################
RUN pip3 install -vU https://github.com/CastXML/pygccxml/archive/develop.zip pyplusplus && \
    pip3 install -vU PyOpenGL-accelerate

WORKDIR /home/open/work

# # 這裡是正確掛 motor_control
# COPY motor_control/ motor_control/

# # 進入motor_control資料夾
# WORKDIR /home/open/work/motor_control

# # ✅ 在 motor_control 目錄下，clone dynamixel_sdk ROS2版本
# RUN git clone -b ros2-devel https://github.com/ROBOTIS-GIT/DynamixelSDK.git dynamixel_sdk

# # 回到workspace
# WORKDIR /home/open/work

# # rosdep install
# RUN rosdep init && rosdep update
# RUN apt-get update && rosdep install --from-paths motor_control --ignore-src -r -y




############################## SWITCH USER ###################################
USER ${USER}
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc && \
    echo "export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp" >> ~/.bashrc && \
    echo "alias ll='ls -alF'" >> ~/.bashrc && \
    echo "export LANG=zh_TW.UTF-8" >> ~/.bashrc && \
    echo "export TZ=Asia/Taipei" >> ~/.bashrc

RUN mkdir -p /home/${USER}/work && sudo chown ${USER}:${GROUP} /home/${USER}/work
WORKDIR /home/${USER}/work


EXPOSE 22
ENTRYPOINT ["terminator"]
