# Docker API Client 
This is an attempt to create client for docker api library for C++. This library implements Docker REST API version `1.41`. </br>
You can read official docker REST API docs [here](https://docs.docker.com/engine/api/v1.41/#tag/Exec/operation/ContainerExec).

## Dependencies
- libfmt
- libcurl
- libtar

## Build
Use cmake:
```
mkdir build && cd build
cmake ..
make
```

## API methods implementation status
- [x] List containers
- [x] (part)Create container
- [x] Inspect container
- [x] List processes inside container
- [x] (part)Container logs
- [ ] Container FS changes
- [ ] Export container
- [ ] Container status (resources usage)
- [ ] Resize Container TTY
- [x] Start container
- [x] Stop container
- [x] Restart container
- [x] Kill container
- [ ] Update container
- [ ] Rename container
- [x] (part) Remove container
- [ ] Pause container
- [ ] Unpause container
- [ ] Attach to container
- [x] Exec bash in container
- [x] (part) Wait for a container