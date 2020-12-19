from graphviz import *


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


e = Digraph('ER', filename='er.gv', engine='sfdp')

e.node('Lender', shape='none', label=label('Lender', ['ID_person', 'name', 'address'], 'ID_person'))
e.node('Borrower', shape='none', label=label('Borrower', ['ID_person', 'name', 'address'], 'ID_person'))
e.node('Charity', shape='none', label=label('Charity', ['ID_person', 'name', 'address'], 'ID_person'))
e.node('Payment History', shape='none', label=label('payment_history', ['date', 'amount'], 'date'))
e.node('Deadline History', shape='none', label=label('deadline_history', ['agreed_date', 'deadline'], 'agreed_date'))
e.node('Loan', shape='none', label=label('Loan', ['loan_date'], 'loan_date'))
e.node('Loan Request', shape='none',
       label=label('Loan request', ['request_date', 'deadline', 'total amount($)', 'payback period', 'description'],
                   'request_date'))

e.node('Score', shape='box', label="Borrower_Score \n (0~100) \n")
e.node('Trust', shape='diamond', label='trust')
e.node('Commit', shape='diamond', label='commit')
e.node('Intermediate', shape='diamond', label='intermediate')
e.node('Portion', shape='box', label='portion')

e.node('Request', shape='diamond', peripheries='2', label='request')
e.node('Convert', shape='diamond', peripheries='2', label='convert')
e.node('Deadline', shape='diamond', peripheries='2', label='deadline')
e.node('Payment', shape='diamond', peripheries='2', label='payment')

e.edge('Score', 'Trust', style='dashed', arrowhead='none')
e.edge('Lender', 'Trust', arrowhead='none')
e.edge('Borrower', 'Trust', arrowhead='none')

e.edge('Request', 'Borrower', arrowhead='normal')
e.edge('Loan Request', 'Request', color="black:black", arrowhead='none')
e.edge('Lender', 'Commit', arrowhead='none')
e.edge('Commit', 'Portion', style='dashed', arrowhead='none')
e.edge('Loan Request', 'Commit', arrowhead='none')

e.edge('Convert', 'Loan Request', arrowhead='normal')
e.edge('Convert', 'Loan', color="black:black", arrowhead='none')
e.edge('Deadline', 'Loan', arrowhead='normal')
e.edge('Payment', 'Loan', arrowhead='normal')
e.edge('Intermediate', 'Loan', color="black:black", arrowhead='none')
e.edge('Intermediate', 'Charity', arrowhead='normal')

e.edge('Deadline', 'Deadline History', color="black:black", arrowhead='none')
e.edge('Payment', 'Payment History', color="black:black", arrowhead='none')

e.render("test_out\hw5-3", view = True)