from cs50 import get_string

# get user input for text to evaluate
text = get_string("Text: ")

#calculate amount of letters in text
letterCount = 0
for c in text:
    if c.isalpha():
        letterCount += 1

#calculate number of words in text
wordCount = 1
for c in text:
    if c.isspace():
        wordCount += 1

#calculate number of sentences in text
sentenceCount = 0
for c in text:
    if c == "." or c == "?" or c == "!":
        sentenceCount += 1

'''calculate index = 0.0588 * L - 0.296 * S - 15.8
   where L is the average number of letters per 100 words in
   the text and S is the average number of sentences per 100
   words in the text'''

l = (float(letterCount) / float(wordCount)) * 100
s = (float(sentenceCount) / float(wordCount)) * 100

indexExact = (0.0588 * l - 0.296 * s - 15.8)
index = round(indexExact)

# now map index to grade level and print
if (index > 15):
    print("Grade 16+")
elif (index < 1):
    print("Before Grade 1")
else:
    print(f"Grade {index}")





