module Frequency
    function is_alphanumeric(c::Char)
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9')
    end

    function is_wordbreak(c::Char)
        # space, hiphen, em-dash, en-dash
        return isspace(c) || (c == '-') || (c == '—') || (c == '–')
    end

    function trimWord(word::String)
        if !is_alphanumeric(word[1])
            ai = 1
            for (i, c) in pairs(word)
                if (is_alphanumeric(word[i]))
                    ai = i
                    break
                end
            end
            word = SubString(word, ai, lastindex(word))
        end

        if length(word) == 0
            return ""
        end

        if !is_alphanumeric(word[lastindex(word)])
            ai = lastindex(word)
            for (i, c) in Iterators.reverse(pairs(word))
                if (is_alphanumeric(word[i]))
                    ai = i
                    break
                end
            end
            word = SubString(word, firstindex(word), ai)
        end
        return word
    end

    function to_csv(map::Dict)
        println("word,usage")
        for k in keys(map)
            println("$(k),$(map[k])")
        end
    end

    export is_alphanumeric, is_wordbreak, trimWord, to_csv
end
