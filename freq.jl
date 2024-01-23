import Base.Threads.@spawn

# pull in the module from next door
include("frequency.jl")
using .Frequency

if (length(ARGS) < 1)
    error("usage: freq.jl [filename]")
    exit(1)
end

function freq(filepath::String)
    words = Dict{String, Int64}()
    fp = open(filepath)

    word = ""
    for chr in readeach(fp, Char)
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
    return words
end

function syncCombineWordLists(l, dest, source)
    lock(l)
    try
        for word in keys(source)
            dest[word] = get(dest, word, 0) + source[word]
        end
    finally
        unlock(l)
    end
end


l = ReentrantLock()
totalWords = Dict{String, Int64}()
threads = Vector{Task}()
for fn in ARGS
    t = @spawn begin
        words = freq(fn)
        println(fn)
        syncCombineWordLists(l, totalWords, words)
    end
    push!(threads, t)
end

for t in threads
    wait(t)
end

# to_csv(totalWords)
