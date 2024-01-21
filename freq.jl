function is_alphanumeric(c::Char)
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9')
end

function trimWord(word::String)
    if !is_alphanumeric(word[1])
        dec = 1
        while !is_alphanumeric(word[1 + dec])
            dec += 1
            if dec == length(word)
                break
            end
        end

        word = SubString(word, 1 + dec, length(word))
    end

    if length(word) == 0
        return ""
    end

    if !is_alphanumeric(word[length(word)])
        dec = 1
        while !is_alphanumeric(word[length(word) - dec])

            dec += 1
            if dec == length(word)
                break
            end

        end

        word = SubString(word, 1, length(word) - dec)
    end
    return word
end

function to_csv(map::Dict)
    for k in keys(map)
        println("$(k), $(map[k])")
    end
end


if (length(ARGS) < 1)
    error("usage: freq.jl [filename]")
    exit(1)
end
words = Dict{String, Int64}()
fp = open(ARGS[1])

word = ""
for chr in readeach(fp, Char)
    global word
    if !isspace(chr)
        word = word * chr
    elseif word != ""
        word = lowercase(word)

        newWord = trimWord(word)
        word = newWord

        words[word] = get(words, word, 0) + 1
        word = ""
    end
end

close(fp)
to_csv(words)
