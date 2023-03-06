# Join server
## Server performs intersection and symmetric difference operations on sets
### Using
```
join_server <port> [mode]
```
- **port** - tcp port number for incoming connections.
- **mode** - operation mode: *line* or *symbolic* (optional, *symbolic* by default).

### List of commands
- INSERT table id name
- TRUNCATE table
- INTERSECTION
- SYMMETRIC_DIFFERENCE

Table names: A and B.