FROM southclaws/sampctl:1.8.39
COPY . /samp
WORKDIR /samp
RUN sampctl p ensure && sampctl p build