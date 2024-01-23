import Base.Threads.@spawn

# pull in the module from next door
include("frequency.jl")
using .Frequency

if (length(ARGS) < 1)
    error("usage: freq.jl [filename]")
    exit(1)
end

# function for calculating the word frequency for a file
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

# create threads for all files
totalWordsLock = ReentrantLock()
totalWords = Dict{String, Int64}()
threads = Vector{Task}()
for fn in ARGS
    t = @spawn begin
        words = Dict{String, Int64}()
        try
        words = freq(fn)
        catch Exception
            @warn "Failed to parse $(fn)!"
            return
        end
        # lock dictionary for insertions individually, so multiple threads can append at the same time
        for word in keys(words)
            lock(totalWordsLock)
            try
                totalWords[word] = get(totalWords, word, 0) + words[word]
            finally
                unlock(totalWordsLock)
            end
        end
    end
    push!(threads, t)
end

# make sure each thread completes
for t in threads
    wait(t)
end

to_csv(totalWords)
