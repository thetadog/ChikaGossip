# ChikaGossip
Gossip protocol implementation in C++

## Assumption
- When a new node joins an existing network, it will be given a pair of [ip, port] of existing node of the network. This existing node is always assumed to be functioning and no failure or de-sync could happen to it throughout the whole creation procedural with the new node.
## Design
- 2 Threads
  - Membership Thread (Active)
    - randomly send pull/push requests with membership table to other nodes
    - wait for pull response
  - Listening Thread (Passive)
    - on receipt of Server, update membership
- Node Creation
  - New nodes in an existing cluster must be given the address of at least one existing member in order to join the cluster
  - Existing node add new node to list, The new node does a full state sync with the existing member.
- Timeout

### Node Local Structure
Node stores its config and membership locally.
at startup, new node sends its config to existing node, existing node sends back entire membership.

#### Node
- ServerNode.class - `self`
  - This is the main body of the server, it spawns threads and handle all things except argument parsing and incoming sockets receiving, which is handled by main.
- NodeConfig.class - `selfConfig`
  - This is given as args at the time of startup. It stores the NodeConfig of this node.
- Membership.class - `members`
  - `members` stores all the NodeConfigs, represented by Membership class, of all nodes in its network.
  - Membership information is exchangeable between nodes and should be pass to a new node during creation, if current node is pointed as the existing node.

## CMD
If a server is not given an existing ip and port, it will assume it's the first node in the network and create a separate graph.
./server ip port [existing_ip existing port]