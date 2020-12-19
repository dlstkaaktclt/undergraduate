import graphviz
from graphviz import Graph
from graphviz import Digraph

def label(tableName, attributes, key):
    beginTable = '<table border="1" cellborder="0" cellspacing="1">'
    endTable = '</table>'

    beginCenter = '<tr align="center"><td>'
    beginLeft = '<tr align="left"><td>'
    endLine = '</td></tr>'

    titleLine = beginCenter + tableName + endLine
    titleTable = beginCenter + beginTable + titleLine + endTable + endLine

    attributeLines = ''

    for attribute in attributes:
        if attribute == key:
            attributeLines += beginLeft + '<u>' + attribute + '</u>' + endLine
        else:
            attributeLines += beginLeft + attribute + endLine

    attributeTable = beginCenter + beginTable + attributeLines + endTable + endLine

    label = '<' + beginTable + titleTable + attributeTable + endTable + '>'

    print(label)

    return label


erd = Graph('ER', filename='gviz/hw5_1.gv', engine='sfdp', node_attr={'shape':'record'})

erd.node('patient', label=label('patient', ['id', 'name', 'log'], 'id'))
erd.node('doctor', label=label('doctor', ['id', 'name', 'rank', 'position'], 'id'))

erd.node('information',
         label=label('information', ['test_number', 'name', 'date', 'patient_id', 'doctor_id'], 'test_number'))

erd.attr('node', shape='diamond')
erd.node('test_and_examination')

erd.edge('test_and_examination', 'doctor', len='0.5', label='1')
erd.edge('test_and_examination', 'patient', len='0.5', label='m')
erd.edge('test_and_examination', 'information', len='0.3', style='dashed')

erd.attr(label=r'\n\nHW5-1 hospital')
erd.attr(fontsize='15')

erd.render(view=True)
