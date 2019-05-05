# kooy
Network Layer for the nodes in a network to share the control data securely

The project consists of a API layer & Network layer to communicate each other.

Ideally a component starts the Kooy and adds Interested topics to get updates from.

The Kooy Hub is responsible for the transportation of the data between nodes securely. The Hub transmits the necessary data to the caller component.

Todo: Adapt https://github.com/berkus/msgpack for serialised data
