To start the docker container to run the debian operating system run

- docker-compose -f docker-compose.yaml up -d

To start an interactive terminal in the started container run

- docker exec -it $containerName /bin/bash

where $containerName is the name of the previously started container, which can be found by running docker ps
