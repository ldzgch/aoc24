datadir = "../data/"

require("table")

function day1 (part2)
    io.input(datadir.."data1.txt")
    list1 = {}
    list2 = {}
    while true do
        num1 = io.read("n")
        num2 = io.read("n")
        if num1 == nil or num2 == nil then break end
        table.insert(list1, num1)
        table.insert(list2, num2)
    end
    sum = 0
    if not part2 then
        table.sort(list1)
        table.sort(list2)
        for i=1, #list1 do
            sum = sum + math.abs(list1[i] - list2[i])
        end
    else
        counts = {}
        for i=1, 100000 do
            counts[i] = 0
        end
        for i=1, #list2 do
            counts[list2[i]] = counts[list2[i]] + 1
        end
        for i=1, #list1 do
            sum = sum + list1[i] * counts[list1[i]]
        end
    end
    print(sum)
end

function day5 (part2)
    io.input(datadir.."data5.txt")
    print(sum)
end

exercises = {}
exercises["1"] = {day1, false}
exercises["1.5"] = {day1, true}
exercises["5"] = {day5, false}
exercises["5.5"] = {day5, true}


if arg[1] == nil then
    print("Provide exercise number")
    os.exit(1)
end
ex = exercises[arg[1]]
if ex ~= nil then
    ex[1](ex[2])
end