#include <iostream>
#include <vector>
#include <string>
#include "Blockchain.hpp"
#include "Node.hpp"

class UserInterface
{
public:
    UserInterface(Node& node);
    void runCommandLineInterface();

private:
    Node& m_node;

    void lookAtBlocks();
    void addBlock();
};

UserInterface::UserInterface(Node& node)
  : m_node(node)
{
}

void UserInterface::runCommandLineInterface()
{
    for (int i = 0; i < 20; ++i) {
        std::cout << "\n(1) Look at Blocks \n(2) Add block\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                lookAtBlocks();
                break;
            case 2:
                addBlock();
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::lookAtBlocks()
{
    std::cout << "What Block do you want to look at? ";
    int blockIndex;
    std::cin >> blockIndex;
    try {
        m_node.getBlockchain().getBlock(blockIndex).toString();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UserInterface::addBlock()
{
    std::cout << "\nADDING BLOCKS!\nEnter your message: ";
    std::string message;
    std::cin.ignore();
    std::getline(std::cin, message);

    try {
        Blockchain& blockchain = m_node.getBlockchain();
        if (blockchain.getNumOfBlocks() == 0) {
            std::cout << "----------------------------------"
                         "\nPlease join the network... Your "
                         "Blockchain doesn't have any blocks "
                      << std::endl;
            return;
        }

        auto [hash, nonce] = proofOfWork(blockchain.getNumOfBlocks(),
                                         blockchain.getLatestBlockHash(),
                                         { message });
        blockchain.addBlock(blockchain.getNumOfBlocks(),
                            blockchain.getLatestBlockHash(),
                            hash,
                            nonce,
                            { message });
        m_node.sendNewChain(blockchain.toJSON());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
