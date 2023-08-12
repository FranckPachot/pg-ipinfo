docker build -t $(basename $PWD) . && docker run -it --name $(basename $PWD) $(basename $PWD)
