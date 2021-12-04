# ChikaGossip
Gossip protocol implementation in C++

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

## CMD
./Server ip port [existing_ip existing port]