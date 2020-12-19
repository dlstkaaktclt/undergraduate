import graphviz
from graphviz import Graph


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


er = Graph('ER', filename='gviz/hw5_2.gv', engine='sfdp', node_attr={'shape':'record'})

# entities
er.node('researcher', label=label('researcher', ['id',  'year_of_birth', 'current_position', 'highest_degree'], 'id'))
er.node('professor', label=label('professor', ['id', 'name', 'rank'], 'id'))
er.node('lab_assistant', label=label('lab_assistant', ['id', 'name'], 'id'))
er.node('institution', label=label('institution', ['name', 'country', 'inaugration_year'], 'name'))
er.node('school', label=label('school', ['name'], 'name'))
er.node('paper', label=label('paper', ['title', 'researcher_name'], 'title'))
er.node('project', label=label('project', ['title', 'start_date', 'end_date'], 'title'))

# relations
er.attr('node', shape='diamond')
er.node('involve', label='involve')
er.node('authored', label='authored')
er.node('employ', label='employ')
er.node('belong', label='belong')
er.node('researcher_type_is', label='researcher_type_is', shape='point')

er.edge('employ', 'institution', '1')
er.edge('researcher', 'employ', 'm')
er.edge('belong', 'school', 'm')
er.edge('belong', 'institution', '1')
er.edge('authored', 'researcher', 'm')
er.edge('authored', 'paper', 'n')
er.edge('involve', 'project', 'm')
er.edge('involve', 'professor', 'n')
er.edge('researcher_type_is', 'professor', '0..1')
er.edge('researcher_type_is', 'lab_assistant', '0..1')
er.edge('researcher_type_is', 'researcher', '1')

# relation attribute
er.attr('node', shape='box')
er.node('grant_money', 'grant_money')
er.edge('grant_money', 'involve', style='dashed')

er.node('researcher_school', label=label('research_school', ['school', 'start_date', 'end_date', 'position'], 'school'))

er.edge('researcher_school', 'employ', style='dashed')


er.attr(label=r'\n\nHW5-2 academic world')
er.attr(fontsize='15')

er.render(view=True)