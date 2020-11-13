// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher(const std::string& key){
    setKey(key);
}

void PlayfairCipher::setKey( const std::string& key){
    
    // store the original key
    key_=key;

    // Append the alphabet
    key_+="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(std::begin(key_), std::end(key_), std::begin(key_), ::toupper);

    // Remove non-alpha chracters
   key_.erase( std::remove_if(std::begin(key_), std::end(key_), [] (char c) {return !isalpha(c);}), key_.end());

    // Change J -> I
    std::transform(std::begin(key_), std::end(key_), std::begin(key_), [] (char c){ return (c=='J') ? 'I' : c ; });

    // Remove duplicated letters
    std::string knownLetters{""};
    auto isKnown = [&] (char c) {
        if (knownLetters.find(c)==std::string::npos){
            knownLetters+=c;
            return false;
        }
        else {
            return true;
        }
    };

    auto iter1 = std::remove_if(std::begin(key_), std::end(key_), isKnown );
    key_.erase(iter1,key_.end());

    // print out to verify
    std::cout << "Known letters: " << knownLetters << std::endl << "Key: " << key_ << " of size: " << key_.size() <<std::endl;

    // Store the coords of each letter in the cipher key map
    size_t position{0};
    for (const char c : key_) {
        // Get the position of a Letter in the key
        position=key_.find(c);
        // Map onto two ints(size_ts) and do reverse due to the limitations of maps (need an inverse)
        Letter2CoordsMap_.insert( {c, {position/5,position%5}});
        Coords2LetterMap_.insert( {{position/5, position%5}, c});
    }
}

std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode CipherMode) const
{
    //temporary declarations to escape the -Wunused-variable error flag
    std::string input{""};

    //  Temporary message
    std::cout << "[warning] Playfair cipher not implemented yet, will return the input string" << std::endl;

    // Change J-> I
    std::transform(std::begin(inputText), std::end(inputText), std::begin(input), [] (char c){ return (c=='J') ? 'I' : c ; });

    // If repeated chars in a digraph, add an X (or Q if XX)
    std::string separetedRepetitions{""};
    for (auto iter = input.begin(); iter!=input.end();iter++){
        separetedRepetitions+=*iter;
        if (*iter==*(iter+1)){
            if (*iter=='X'){ separetedRepetitions+='Q'; }
            else { separetedRepetitions+='X'; }
        }
    }
    // Add 'Z' at the end if number of letters sotred is odd
    if (separetedRepetitions.size()%2==1) { separetedRepetitions+='Z';}
    //Clear input to be able to append to it later
    input="";
    
    // Loop over the input digraphs
    for (auto iter = separetedRepetitions.begin(); iter!=separetedRepetitions.end(); iter+=2){

        //  - Find the coords in the grid for each digraph
        auto point1 = Letter2CoordsMap_.find(*iter);
        auto point2 = Letter2CoordsMap_.find(*(iter+1));
        size_t x1 = point1->second.first;
        size_t y1 = point1->second.second;
        size_t x2 = point2->second.first;
        size_t y2 = point2->second.second;

        //  - Apply the rules to these coords to get 'new' coords
        //  - Find the letter associated with the new coords
        if (x1==x2){
            input += (*Coords2LetterMap_.find({(x1+1)%5, y1})).second;
            input += (*Coords2LetterMap_.find({(x2+1)%5, y2})).second;
        }
        else if (y1==y2){
            input += (*Coords2LetterMap_.find({x1, (y1+1)%5})).second;
            input += (*Coords2LetterMap_.find({x2, (y2+1)%5})).second;
        }
        else {
            input += (*Coords2LetterMap_.find({abs(x1-4), y1})).second;
            input += (*Coords2LetterMap_.find({abs(x2-4), y2})).second;
        }
    }

    // Delete the X's and Q's separating 'double' letters 
    // Sadly, nothing can be done about the Z at the end of odd-lettered encripted text 
    if (CipherMode==CipherMode::Decrypt){
        std::string decrypted{""};
        for (auto iter = input.begin(); iter!=input.end(); iter += 2 ){
            decrypted+=*iter;
            if (*iter==*(iter +2) && (*(iter+1)=='X' || *(iter+1)=='Q')) { continue; }
            else { decrypted += *(iter+1);}
        }
        return decrypted;
    }
    // return the text
    else{ 
        return input;
    }
}


