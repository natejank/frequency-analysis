import Base.Threads.@spawn

# pull in the module from next door
include("frequency.jl")
using .Frequency

"""
    freq(filepath)

Compute the word frequency in the file at `filepath`.  The result is returned
as a Dict{String, Int64}.

`filepath` is assumed to be a valid file encoded as UTF-8.
"""
function freq(filepath::String)
    words = Dict{String, Int64}()
    fp = open(filepath)

    word = ""
    for chr in readeach(fp, Char)
        if !is_wordbreak(chr)
            word = word * chr
        elseif word != ""
            word = lowercase(word)
            word = trimWord(word)

            words[word] = get(words, word, 0) + 1
            word = ""
        end
    end
    close(fp)
    return words
end

if (length(ARGS) < 1)
    error("usage: freq.jl [filename]")
    exit(1)
end

# aggregate word map
totalWords = Dict{String, Int64}()
totalWordsLock = ReentrantLock()

# keep track of the threads we spawn
threads = Vector{Task}(undef, length(ARGS))
for (i, filename) in pairs(ARGS)
    # create threads for all files
    t = @spawn begin
        threadWords = Dict{String, Int64}()
        try
            threadWords = freq(filename)
        catch Exception
            # freq might fail if the encoding is non-standard.  It's easiest
            # just to skip that input.
            @warn "Failed to parse $(filename)!"
            return
        end

        # lock dictionary for insertions individually, so multiple threads can append at the same time
        for word in keys(threadWords)
            lock(totalWordsLock)
            try
                totalWords[word] = get(totalWords, word, 0) + threadWords[word]
            finally
                unlock(totalWordsLock)
            end
        end
    end
    threads[i] = t
end

# make sure each thread completes
for t in threads
    wait(t)
end

to_csv(totalWords)
