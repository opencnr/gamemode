FROM southclaws/sampctl:1.8.39

# Generate ~/.samp directory
RUN sampctl
# Disable Sampctl metrics in config
RUN sed -i 's/"metrics": true/"metrics": false/' ~/.samp/config.json

WORKDIR /samp

COPY pawn.json pawn.json
RUN sampctl p ensure

ARG build=dev
COPY . .
RUN sampctl p build ${build}

# Use the following command to build for production :
# docker-compose build --build-arg build=prod 
