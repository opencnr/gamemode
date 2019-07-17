FROM southclaws/sampctl:1.8.39

WORKDIR /samp

COPY ./pawn.json /samp/pawn.json
RUN sampctl p ensure

COPY . /samp
RUN sampctl p build