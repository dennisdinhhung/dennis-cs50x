import csv
import sys

def main():
    # require to use sys.argv

    if len(sys.argv) != 3:
        sys.exit("USAGE: python dna.py LISTNAME SEQUENCENAME")

    STR_set = [] # to store STR sets
    people = []

    # open csv file
    with open(sys.argv[1]) as file_csv:

        # obtain STR sets from csv's first line
        row_csv = csv.reader(file_csv)
        STR_set = next(row_csv)
        
    with open(sys.argv[1]) as file_csv:
        # read csv file as a dict
        reader_csv = csv.DictReader(file_csv)
        for name in reader_csv:
            for i in range(1, len(STR_set)):
                name[STR_set[i]] = int(name[STR_set[i]])
            people.append(name)
        

    # open dna file
    with open(sys.argv[2]) as file_dna:
        dna = file_dna.read()

        # go thru the STR_set and use its value
        # once got the model set, scan thru the dna and look for said model set
        # count the repeats and note down the highest repeat number
        counts = {}
        for i in range(1, len(STR_set)):
            temp = STR_set[i]
            count = 0
            high = 0
            a = 0

            # loop over the dna with a as pivot point and count the sets
            while a < len(dna):
                set_current = dna[a:a+len(temp)]
                set_prev = dna[a-len(temp):a]
                set_after = dna[a+len(temp):a+len(temp)*2]
                if set_current == temp and set_prev == set_current:
                    count += 1
                    if high < count:
                        high = count
                    a += len(temp)
                elif set_current == temp and set_after == set_current:
                    count += 1
                    if high < count:
                        high = count
                    a += len(temp)
                elif set_current == temp: # if there is only 1 set that matches
                    count += 1
                    if high < count:
                        high = count
                    a += len(temp)
                else:
                    a += 1
                    count = 0

            # if the model set is in counts{} the update the value, if not, make a new value for the model set
            if STR_set[i] in counts:
                counts[STR_set[i]] = high
            else:
                counts[STR_set[i]] = high

        # compare the counts[] with the csv data to find the person
        for person in people: # loop over each person in people list of dict
            correct = 0
            for j in range(1,len(STR_set)):
                if person[STR_set[j]] == counts[STR_set[j]]:
                    correct += 1
            if correct == len(STR_set) - 1:
                print(person["name"])
                return 0

        # print "No match" if there are no one that fits the DNA
        print("No match")
        return 0
    
main()