#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:

    /**
     * Create a new PlayfairCipher, converting the given string into the key
     *
     * \param key the string to set as the key to be used in the cipher
     */
    PlayfairCipher( const std::string& key );

    /**
     * Set the key of the cipher based on the input string
     * 
     * \param key string to be turned into the key 
     */

    void setKey(const std::string& key);

    /**
     * Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

  private:
    /// The cipher key
    std::string key_ = "";

    /// Map connecting the letters of the cipher key to their coordinates
    std::map <char, std::pair<size_t,size_t>> Letter2CoordsMap_;
    /// Map connecting the coordinates of a cipher key to their letter
    std::map <std::pair<size_t,size_t>, char> Coords2LetterMap_; 
};

#endif