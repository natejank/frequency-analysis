function is_alphanumeric(c::Char)
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9')
end

function trimWord(word::String)
    if !is_alphanumeric(word[1])
        word = SubString(word, 2, length(word))
    end
    if !is_alphanumeric(word[length(word)])
        word = SubString(word, 1, length(word) - 1)
    end
    return word
end

function to_csv(map::Dict)
    for k in keys(map)
        println("$(k), $(map[k])")
    end
end


t = Dict{String, Int64}()
fp = open("examples/interjection.txt")

word = ""
for chr in readeach(fp, Char)
    global word
    if !isspace(chr)
        word = word * chr
    elseif word != ""
        word = lowercase(word)
        word = trimWord(word)

        t[word] = get(t, word, 0) + 1
        word = ""
    end
end

close(fp)
to_csv(t)
