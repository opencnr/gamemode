FROM southclaws/sampctl:1.8.39

WORKDIR /samp

COPY ./pawn.json /samp/pawn.json
RUN sampctl p ensure

ARG build=dev
COPY . /samp
RUN sampctl p build ${build}

# Use the following command to build for production :
# docker-compose build --build-arg build=prod 
