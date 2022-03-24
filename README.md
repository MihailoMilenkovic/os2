To start the docker container for the operating systems

- docker-compose -f docker-compose.yaml up -d

To start an interactive terminal in the started container run

- docker exec -it $containerName /bin/bash
