FROM southclaws/sampctl:1.8.39

# Generate ~/.samp directory
RUN sampctl
# Disable Sampctl metrics in config
RUN sed -i 's/"metrics": true/"metrics": false/' ~/.samp/config.json

WORKDIR /samp

COPY pawn.json pawn.json
RUN sampctl p ensure

ARG build=prod
COPY . .
RUN sampctl p build ${build}

# Use the following command to build for development :
# docker-compose build --build-arg build=dev
