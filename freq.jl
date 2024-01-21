# pull in the module from next door
include("frequency.jl")
using .Frequency

if (length(ARGS) < 1)
    error("usage: freq.jl [filename]")
    exit(1)
end

words = Dict{String, Int64}()
fp = open(ARGS[1])

word = ""
for chr in readeach(fp, Char)
    global word
    if !is_wordbreak(chr)
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
